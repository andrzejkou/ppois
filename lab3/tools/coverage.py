#!/usr/bin/env python3
"""Run CTest and enforce minimum aggregate and per-source GCC line coverage."""
from __future__ import annotations
import argparse
import re
import subprocess
import tempfile
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", required=True, type=Path)
    parser.add_argument("--source-dir", required=True, type=Path)
    parser.add_argument("--min-line-coverage", type=float, default=90.0,
                        help="minimum aggregate line coverage (default: 90)")
    parser.add_argument("--min-file-line-coverage", type=float, default=90.0,
                        help="minimum line coverage required in every cpp source file (default: 90)")
    args = parser.parse_args()
    build = args.build_dir.resolve()
    source = args.source_dir.resolve()

    if not 0.0 <= args.min_line_coverage <= 100.0:
        parser.error("--min-line-coverage must be between 0 and 100")
    if not 0.0 <= args.min_file_line_coverage <= 100.0:
        parser.error("--min-file-line-coverage must be between 0 and 100")

    # Remove old counters so previous runs cannot inflate the result.
    for counter in build.rglob("*.gcda"):
        counter.unlink()

    test = subprocess.run(
        ["ctest", "--test-dir", str(build), "--output-on-failure"],
        check=False,
    )
    if test.returncode:
        return test.returncode

    total = hit = 0
    rows: list[tuple[float, str, int, int]] = []
    gcov_errors: list[str] = []

    with tempfile.TemporaryDirectory(prefix="warehouse-gcov-") as tmp:
        output = Path(tmp)
        for cpp in sorted((source / "cpp").rglob("*.cpp")):
            relative = cpp.relative_to(source)
            obj = build / "CMakeFiles" / "warehouse.dir" / relative.with_suffix(".cpp.o")
            if not obj.exists():
                continue

            run = subprocess.run(
                ["gcov", "-b", "-c", "-o", str(obj), str(cpp)],
                cwd=output, text=True, capture_output=True, check=False,
            )
            gcov_file = output / f"{cpp.name}.gcov"
            if run.returncode != 0 or not gcov_file.exists():
                gcov_errors.append(str(relative))
                continue

            # gcov may emit the same physical source line more than once when
            # several functions are written on one line. Count each source line
            # once and mark it covered if any executable block on that line ran.
            physical_lines: dict[int, bool] = {}
            for line in gcov_file.read_text(errors="replace").splitlines():
                match = re.match(r"\s*(\d+|#####|=====|-):\s*(\d+):", line)
                if not match or match.group(1) == "-":
                    continue
                count, line_number = match.groups()
                covered = count not in {"#####", "====="} and int(count) > 0
                physical_lines[int(line_number)] = physical_lines.get(int(line_number), False) or covered

            file_total = len(physical_lines)
            file_hit = sum(physical_lines.values())
            if file_total:
                total += file_total
                hit += file_hit
                rows.append((100.0 * file_hit / file_total, str(relative), file_hit, file_total))

    if not total:
        print("ERROR: gcov did not produce any source-line data.")
        return 2

    percentage = 100.0 * hit / total
    report = [
        "Warehouse Management — GCC line coverage",
        f"Line coverage: {hit}/{total} = {percentage:.2f}%",
        f"Required aggregate minimum: {args.min_line_coverage:.2f}%",
        f"Required per-file minimum: {args.min_file_line_coverage:.2f}%",
        f"Aggregate result: {'PASS' if percentage >= args.min_line_coverage else 'FAIL'}",
        "",
        "Per-file line coverage:",
    ]
    for pct, name, covered, lines in sorted(rows):
        status = "PASS" if pct >= args.min_file_line_coverage else "FAIL"
        report.append(f"[{status}] {name}: {covered}/{lines} ({pct:.1f}%)")
    if gcov_errors:
        report += ["", "Files without gcov data:", *gcov_errors]

    result = "\n".join(report) + "\n"
    print(result)
    (build / "coverage-summary.txt").write_text(result)

    failing_files = [row for row in rows if row[0] < args.min_file_line_coverage]
    if percentage < args.min_line_coverage or failing_files or gcov_errors:
        if percentage < args.min_line_coverage:
            print("ERROR: aggregate line coverage is below the required threshold.")
        if failing_files:
            print("ERROR: one or more source files are below the per-file line-coverage threshold.")
        if gcov_errors:
            print("ERROR: gcov data is missing for one or more source files.")
        return 1
    print(f"PASS: every source file meets {args.min_file_line_coverage:.2f}% line coverage.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
