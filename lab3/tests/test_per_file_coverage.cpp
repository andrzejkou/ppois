#include "exceptions/WarehouseException.h"
#include "inventory/StockReservation.h"
#include "inventory/StockItem.h"
#include "inventory/StockLot.h"
#include "inventory/Inventory.h"
#include "inventory/InventoryLine.h"
#include "orders/SalesOrderLine.h"
#include "orders/SalesOrder.h"
#include "orders/PickingTask.h"
#include "orders/PackingTask.h"
#include "people/WarehouseWorker.h"
#include "people/QualityInspector.h"
#include "people/Driver.h"
#include "people/Employee.h"
#include "people/Storekeeper.h"
#include "people/WarehouseManager.h"
#include "procurement/DeliveryNote.h"
#include "procurement/PurchaseOrder.h"
#include "procurement/PurchaseRequest.h"
#include "procurement/Supplier.h"
#include "procurement/SupplierContract.h"
#include "procurement/GoodsReceipt.h"
#include "products/Product.h"
#include "products/Barcode.h"
#include "storage/StorageZone.h"
#include "storage/Warehouse.h"
#include "logistics/Delivery.h"
#include "finance/WarehouseExpense.h"
#include <cassert>
#include <cmath>
#include <ctime>
#include <string>
#include <utility>

template<class F> static void throws(F&& f) {
    bool caught = false;
    try { f(); } catch (const std::exception&) { caught = true; }
    assert(caught);
}

int main() {
    // StockReservation: consume path, getters and invalid state / item combinations.
    StockItem stock("ST-1", "SKU-1", "WH-1", "A-1", 12);
    StockReservation consume("RS-1", "SO-1", "SKU-1", 3);
    assert(consume.active() && consume.quantity() == 3);
    assert(consume.id() == "RS-1" && consume.orderId() == "SO-1" && consume.sku() == "SKU-1");
    consume.apply(stock);
    consume.consume(stock);
    assert(!consume.active() && consume.status() == ReservationStatus::Consumed);
    assert(stock.onHand() == 9 && stock.reserved() == 0);
    throws([&] { consume.consume(stock); });
    throws([&] { StockReservation bad("", "O", "S", 1); });

    // SalesOrderLine successful quantity and discount changes, including getters.
    SalesOrderLine sol("SKU-L", 2, 12.5);
    assert(sol.sku() == "SKU-L" && sol.quantity() == 2 && sol.unitPrice() == 12.5);
    assert(sol.discount() == 0.0 && sol.lineTotal() == 25.0);
    sol.changeQuantity(4); sol.applyDiscount(25.0);
    assert(std::abs(sol.lineTotal() - 37.5) < 1e-9);

    // StockLot: not expired, shelf life, matching active/inactive products.
    const std::time_t now = std::time(nullptr);
    StockLot fresh("LOT-F", "SKU-F", 8, now + 4 * 86400, 2.5);
    Product lotProduct("SKU-F", "Fresh", 2.5, 1, "CAT", "BR");
    assert(!fresh.isExpired(now));
    assert(fresh.remainingShelfLifeDays(now) >= 3);
    assert(fresh.matchesProduct(lotProduct));
    lotProduct.deactivate();
    assert(!fresh.matchesProduct(lotProduct));
    assert(fresh.lotNumber() == "LOT-F" && fresh.sku() == "SKU-F");
    assert(fresh.quantity() == 8 && fresh.totalCost() == 20.0);
    throws([&] { StockLot bad("", "SKU", 1, now + 100, 1); });

    // WarehouseWorker: shift gates, both task types, duplicate IDs and completion.
    WarehouseWorker worker("EMP-W", "DAY", 20.0);
    assert(worker.employeeId() == "EMP-W" && worker.shiftCode() == "DAY");
    assert(!worker.startShift(""));
    assert(worker.startShift("2026-11-20"));
    assert(!worker.startShift("2026-11-20"));
    PickingTask pick("PICK-W", "SO-W", 2, 5.0);
    PackingTask pack("PACK-W", "SO-W", 2, 5.0);
    assert(worker.assignPickingTask(pick));
    assert(!worker.assignPickingTask(pick));
    assert(worker.assignPackingTask(pack));
    assert(!worker.finishShift("2026-11-21"));
    assert(!worker.finishShift("2026-11-19"));
    assert(!worker.completeTask("NO-TASK"));
    assert(worker.completeTask("PICK-W") && worker.completeTask("PACK-W"));
    assert(worker.finishShift("2026-11-21") && !worker.onShift());
    assert(worker.calculateShiftPay(8) == 160.0 && worker.calculateShiftPay(-1) == 0.0);
    throws([&] { WarehouseWorker bad("", "DAY", 1); });

    // QualityInspector: both inspection outcomes and empty-lot path.
    QualityInspector inspector("QI-1", "CERT-1", "2027-12-31");
    assert(inspector.employeeId() == "QI-1" && inspector.certificateCode() == "CERT-1");
    assert(inspector.certificateValidOn("2026-11-20"));
    assert(!inspector.certificateValidOn("2028-01-01"));
    assert(!inspector.certificateValidOn("bad"));
    StockLot passLot("LP", "SKU", 2, now + 100000, 1);
    StockLot failLot("LF", "SKU", 2, now + 100000, 1);
    StockLot emptyLot("LE", "SKU", 0, now + 100000, 1);
    failLot.markQuarantined();
    assert(inspector.inspectLot(passLot, true) && !passLot.quarantined());
    assert(inspector.inspectLot(failLot, false) && failLot.quarantined());
    assert(!inspector.inspectLot(emptyLot, true));
    assert(inspector.approveQuality() && inspector.approvedLots() == 1 && inspector.rejectedLots() == 1);
    throws([&] { QualityInspector bad("QI", "CERT", "bad"); });

    // DeliveryNote: constructor, duplicate aggregation, all validation outcomes and getters.
    throws([&] { DeliveryNote bad("", "SUP", "PO"); });
    DeliveryNote note("DN-COV", "SUP-COV", "PO-COV");
    assert(note.noteNumber() == "DN-COV" && note.supplierId() == "SUP-COV" && note.purchaseOrderId() == "PO-COV");
    assert(!note.received() && note.receivedDate().empty());
    assert(!note.addLine({"", 1}) && !note.addLine({"SKU", 0}));
    assert(note.addLine({"SKU", 2}) && note.addLine({"SKU", 3}));
    assert(note.lines().size() == 1 && note.totalDeliveredQuantity() == 5);
    PurchaseOrder po("PO-COV", "SUP-COV");
    po.addLine(PurchaseOrderLine("SKU", 10, 2.0));
    po.approve();
    assert(note.validateAgainstOrder(po));
    PurchaseOrder wrongPo("PO-WRONG", "SUP-COV");
    wrongPo.addLine(PurchaseOrderLine("SKU", 10, 2.0));
    assert(!note.validateAgainstOrder(wrongPo));
    assert(!note.markReceived("bad"));
    assert(note.markReceived("2026-11-20"));
    assert(note.received() && note.receivedDate() == "2026-11-20");
    assert(!note.addLine({"SKU2", 1}) && !note.markReceived("2026-11-20"));

    // Driver: successful delivery lifecycle and metadata access.
    Driver driver("DRV-1", "LIC-1", "2027-12-31", 1200.0);
    assert(driver.employeeId() == "DRV-1" && driver.licenseNumber() == "LIC-1");
    assert(driver.licenseExpiry() == "2027-12-31" && driver.loadLimitKg() == 1200.0);
    assert(driver.validateLicense("2026-11-20") && !driver.validateLicense("bad"));
    assert(driver.canCarry(0) && driver.canCarry(1200) && !driver.canCarry(1201) && !driver.canCarry(-1));
    Delivery delivery("DEL-COV", "SHIP-COV", 2, 5.0);
    assert(driver.assignDelivery(delivery));
    assert(!driver.assignDelivery(delivery));
    assert(!driver.recordDelivery("MISSING"));
    assert(driver.recordDelivery("DEL-COV") && driver.completedDeliveries() == 1);
    throws([&] { Driver bad("DRV", "LIC", "bad", 10); });

    // Employee: getters, skill de-duplication, successful changes and inactive gates.
    Employee employee("EMP-C", "Alex Worker", "Picker", 15.0);
    assert(employee.employeeId() == "EMP-C" && employee.fullName() == "Alex Worker");
    assert(employee.position() == "Picker" && employee.hourlyRate() == 15.0 && employee.active());
    assert(employee.validateIdentity());
    assert(!employee.changePosition(""));
    assert(employee.changePosition("Senior picker"));
    assert(!employee.changeHourlyRate(-1));
    assert(employee.changeHourlyRate(18.0));
    employee.addSkill(""); employee.addSkill("forklift"); employee.addSkill("forklift");
    assert(employee.hasSkill("forklift") && !employee.hasSkill("unknown") && employee.skills().size() == 1);
    employee.deactivate();
    assert(!employee.active() && !employee.validateIdentity());
    assert(!employee.changePosition("Manager") && !employee.changeHourlyRate(20));
    throws([&] { Employee bad("", "Name", "Role", 1); });

    // StorageZone and Storekeeper: all accessors and task-limit paths.
    StorageZone zone("Z-COV", "ambient", 50.0, 10.0);
    assert(zone.code() == "Z-COV" && zone.temperatureMode() == "ambient");
    assert(zone.capacity() == 50.0 && zone.occupied() == 10.0);
    assert(zone.isSuitableFor("ambient") && !zone.isSuitableFor("cold"));
    Storekeeper keeper("SK-1", "Z-COV", 1);
    assert(keeper.employeeId() == "SK-1" && keeper.zoneCode() == "Z-COV");
    assert(keeper.assignZone(zone));
    StorageZone otherZone("Z-OTHER", "cold", 20.0);
    assert(!keeper.assignZone(otherZone));
    PickingTask keeperTask("PICK-SK", "SO-SK", 1, 1.0);
    assert(keeper.canAcceptTask() && keeper.assignPickingTask(keeperTask));
    assert(!keeper.canAcceptTask() && !keeper.assignPickingTask(keeperTask));
    assert(keeper.countAssignedTasks() == 1 && !keeper.completeTask("missing"));
    assert(keeper.completeTask("PICK-SK") && keeper.canAcceptTask());
    throws([&] { Storekeeper bad("SK", "Z", 0); });

    // Supplier / contract: can fulfil, association, renewal and getters.
    Supplier supplier("SUP-COV", "Supplier Co", "s@example.test", 4, 4.0);
    assert(supplier.supplierId() == "SUP-COV" && supplier.companyName() == "Supplier Co");
    assert(supplier.contactEmail() == "s@example.test" && supplier.leadTimeDays() == 4);
    assert(supplier.active() && supplier.reliabilityScore() == 4.0);
    PurchaseRequest request("PR-SUP", "OPS", "EMP");
    assert(request.addLine({"SKU", 2, 3.0}) && request.submit());
    assert(supplier.canFulfil(request));
    SupplierContract contract("SC-1", "SUP-COV", "2026-01-01", "2026-12-31", 10.0, 0.1);
    assert(contract.contractId() == "SC-1" && contract.supplierId() == "SUP-COV");
    assert(contract.minimumOrderValue() == 10.0 && contract.belongsTo(supplier));
    assert(contract.isValidOn("2026-06-01") && !contract.isValidOn("bad") && !contract.isValidOn("2027-01-01"));
    assert(contract.calculatePenalty(2, 100.0) == 20.0);
    assert(contract.calculatePenalty(0, 100.0) == 0.0 && contract.calculatePenalty(2, 5.0) == 0.0);
    assert(!contract.renew("bad") && !contract.renew("2026-12-31"));
    assert(contract.renew("2027-12-31") && contract.isValidOn("2027-01-01"));
    throws([&] { SupplierContract bad("", "SUP", "2026-01-01", "2026-12-31", 0, 0); });

    // PurchaseOrder getters and supplier relation.
    assert(po.id() == "PO-COV" && po.supplierId() == "SUP-COV");
    assert(po.status() == PurchaseOrderStatus::Approved && po.lines().size() == 1);
    assert(po.totalCost() == 20.0 && !po.fullyReceived());
    assert(po.suppliedBy(supplier));
    PurchaseOrder fullyReceivedOrder("PO-FULL", "SUP-COV");
    fullyReceivedOrder.addLine(PurchaseOrderLine("SKU-FULL", 1, 7.0));
    fullyReceivedOrder.approve();
    fullyReceivedOrder.registerReceipt("SKU-FULL", 1);
    assert(fullyReceivedOrder.fullyReceived());
    Supplier otherSupplier("SUP-OTHER", "Other", "o@example.test", 1, 3.0);
    assert(!po.suppliedBy(otherSupplier));

    // InventoryLine: record/reconcile and accessors.
    InventoryLine invLine("SKU-INV", 5);
    assert(invLine.sku() == "SKU-INV" && invLine.expectedQuantity() == 5);
    assert(invLine.countedQuantity() == 0 && !invLine.counted() && !invLine.matches());
    StockItem invStock("ST-INV", "SKU-INV", "WH-INV", "BIN-INV", 5);
    assert(invLine.reconcileWith(invStock) && invLine.matches() && invLine.difference() == 0);
    StockItem wrongInvStock("ST-X", "SKU-X", "WH-INV", "BIN", 5);
    assert(!invLine.reconcileWith(wrongInvStock));
    throws([&] { InventoryLine bad("", 1); });

    // Inventory state and warehouse association.
    Inventory inventory("INV-COV", "WH-COV");
    assert(inventory.id() == "INV-COV" && inventory.warehouseCode() == "WH-COV");
    Warehouse wh("WH-COV", "Address", 100.0);
    Warehouse inactiveWh("WH-COV", "Address", 100.0); inactiveWh.close();
    assert(inventory.belongsTo(wh) && !inventory.belongsTo(inactiveWh));
    assert(inventory.status() == InventoryStatus::Planned && inventory.lines().empty());
    inventory.addLine(InventoryLine("SKU-A", 5));
    inventory.addLine(InventoryLine("SKU-B", 3));
    assert(!inventory.allLinesCounted());
    inventory.start();
    inventory.recordCount("SKU-A", 4);
    inventory.recordCount("SKU-B", 3);
    assert(inventory.allLinesCounted() && inventory.discrepancyCount() == 1);
    assert(inventory.totalDifference() == -1);
    inventory.complete();
    assert(inventory.status() == InventoryStatus::Completed);
    throws([&] { inventory.cancel(); });
    Inventory cancelled("INV-CANCEL", "WH-COV");
    cancelled.cancel();
    assert(cancelled.status() == InventoryStatus::Cancelled);

    // PurchaseRequest getters and both accepted/rejected terminal metadata.
    PurchaseRequest req("PR-GET", "LOG", "EMP-GET");
    assert(req.requestId() == "PR-GET" && req.departmentCode() == "LOG" && req.requesterId() == "EMP-GET");
    assert(req.approverId().empty() && req.rejectionReason().empty() && req.lines().empty());
    assert(req.addLine({"SKU", 1, 2.0}) && req.submit() && req.approve("MGR"));
    assert(req.approverId() == "MGR" && req.status() == PurchaseRequestStatus::Approved);

    // GoodsReceipt metadata getters and accepted-value path.
    PurchaseOrder receiptOrder("PO-GR-COV", "SUP-GR");
    receiptOrder.addLine(PurchaseOrderLine("SKU-GR-COV", 4, 3.0));
    receiptOrder.approve();
    DeliveryNote receiptNote("DN-GR-COV", "SUP-GR", "PO-GR-COV");
    receiptNote.addLine({"SKU-GR-COV", 2});
    GoodsReceipt receipt("GR-COV", "PO-GR-COV", "QI-COV");
    assert(receipt.receiptId() == "GR-COV" && receipt.purchaseOrderId() == "PO-GR-COV");
    assert(receipt.inspectorId() == "QI-COV" && receipt.rejectionReason().empty());
    assert(!receipt.completed() && receipt.acceptedQuantity() == 0 && receipt.rejectedQuantity() == 0);
    assert(receipt.acceptDelivery(receiptNote, receiptOrder, "2026-11-20"));
    assert(receipt.calculateAcceptedValue(receiptOrder) == 6.0);

    // Product getters and barcode registration success, duplicate, invalid, inactive.
    Product product("SKU-BAR", "Barcode product", 4.5, 2, "CAT-B", "BR-B");
    assert(product.sku() == "SKU-BAR" && product.name() == "Barcode product");
    assert(product.unitPrice() == 4.5 && product.reorderLevel() == 2);
    assert(product.categoryCode() == "CAT-B" && product.brandCode() == "BR-B");
    assert(product.active() && product.barcodes().empty());
    Barcode barcode("123456789012", "EAN13");
    assert(product.registerBarcode(barcode));
    assert(product.registerBarcode(barcode));
    product.deactivate();
    assert(!product.registerBarcode(barcode));

    // StockItem getters and product relation.
    StockItem item("ST-COV", "SKU-BAR", "WH-COV", "BIN-COV", 7, 2);
    assert(item.id() == "ST-COV" && item.sku() == "SKU-BAR");
    assert(item.warehouseCode() == "WH-COV" && item.binCode() == "BIN-COV");
    assert(item.onHand() == 7 && item.reserved() == 2 && item.available() == 5);
    product.activate();
    assert(item.matchesProduct(product));
    product.deactivate();
    assert(!item.matchesProduct(product));

    // WarehouseManager getters and positive approval paths.
    WarehouseManager manager("MGR-COV", "PROC", 100.0, 2);
    assert(manager.employeeId() == "MGR-COV" && manager.departmentCode() == "PROC");
    assert(manager.approvalLimit() == 100.0);
    PurchaseRequest mgrRequest("PR-MGR", "PROC", "EMP");
    assert(mgrRequest.addLine({"SKU", 2, 10.0}) && mgrRequest.submit());
    assert(manager.approvePurchaseRequest(mgrRequest));
    WarehouseExpense expense("EXP-COV", "supplies", 1, 20.0);
    assert(manager.approveExpense(expense));
    assert(manager.calculateTeamLoad(4.0) == 0.0);

    // Warehouse accessors and zone membership checks.
    Warehouse warehouse("WH-ACCESS", "Some address", 200.0);
    assert(warehouse.code() == "WH-ACCESS" && warehouse.address() == "Some address");
    assert(warehouse.capacity() == 200.0 && warehouse.occupied() == 0.0);
    assert(warehouse.active() && warehouse.freeCapacity() == 200.0 && warehouse.occupancyRate() == 0.0);
    warehouse.addZone(zone);
    assert(warehouse.containsZone("Z-COV") && !warehouse.containsZone("NO-ZONE"));
    assert(warehouse.zoneCodes().size() == 1);

    return 0;
}
