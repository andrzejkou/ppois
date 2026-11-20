# CMake generated Testfile for 
# Source directory: /home/andrey/projects/PPOIS/lab3
# Build directory: /home/andrey/projects/PPOIS/lab3/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(warehouse_smoke_tests "/home/andrey/projects/PPOIS/lab3/build/warehouse_tests")
set_tests_properties(warehouse_smoke_tests PROPERTIES  _BACKTRACE_TRIPLES "/home/andrey/projects/PPOIS/lab3/CMakeLists.txt;47;add_test;/home/andrey/projects/PPOIS/lab3/CMakeLists.txt;0;")
add_test(warehouse_generated_entities "/home/andrey/projects/PPOIS/lab3/build/warehouse_generated_tests")
set_tests_properties(warehouse_generated_entities PROPERTIES  _BACKTRACE_TRIPLES "/home/andrey/projects/PPOIS/lab3/CMakeLists.txt;51;add_test;/home/andrey/projects/PPOIS/lab3/CMakeLists.txt;0;")
add_test(warehouse_edge_cases "/home/andrey/projects/PPOIS/lab3/build/warehouse_edge_tests")
set_tests_properties(warehouse_edge_cases PROPERTIES  _BACKTRACE_TRIPLES "/home/andrey/projects/PPOIS/lab3/CMakeLists.txt;55;add_test;/home/andrey/projects/PPOIS/lab3/CMakeLists.txt;0;")
add_test(warehouse_per_file_coverage "/home/andrey/projects/PPOIS/lab3/build/warehouse_per_file_tests")
set_tests_properties(warehouse_per_file_coverage PROPERTIES  _BACKTRACE_TRIPLES "/home/andrey/projects/PPOIS/lab3/CMakeLists.txt;59;add_test;/home/andrey/projects/PPOIS/lab3/CMakeLists.txt;0;")
