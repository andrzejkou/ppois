#include "products/Product.h"
#include "storage/Warehouse.h"
#include "storage/StorageZone.h"
#include "inventory/StockItem.h"
#include "inventory/StockReservation.h"
#include "orders/SalesOrder.h"
#include "inventory/Inventory.h"
#include "procurement/PurchaseOrder.h"
#include "exceptions/InsufficientStockException.h"
#include "exceptions/StorageCapacityExceededException.h"
#include "people/Employee.h"
#include "people/Driver.h"
#include "people/QualityInspector.h"
#include "people/Storekeeper.h"
#include "people/WarehouseManager.h"
#include "people/WarehouseWorker.h"
#include "procurement/Supplier.h"
#include "procurement/SupplierContract.h"
#include "procurement/PurchaseRequest.h"
#include "procurement/DeliveryNote.h"
#include "procurement/GoodsReceipt.h"
#include "orders/PickingTask.h"
#include "orders/PackingTask.h"
#include "logistics/Delivery.h"
#include "inventory/StockLot.h"
#include "finance/WarehouseExpense.h"
#include "products/Barcode.h"
#include "products/Brand.h"
#include "products/ProductAttribute.h"
#include "products/ProductBundle.h"
#include "products/ProductCategory.h"
#include "products/SerialNumber.h"
#include "products/UnitOfMeasure.h"
#include "storage/Rack.h"
#include "storage/Shelf.h"
#include "storage/StorageBin.h"
#include "storage/ColdStorage.h"
#include "storage/LoadingDock.h"
#include "storage/StorageCondition.h"
#include "inventory/StockBalance.h"
#include "inventory/StockMovement.h"
#include "inventory/StockAdjustment.h"
#include "inventory/ReorderRule.h"
#include "logistics/Carrier.h"
#include "logistics/Transport.h"
#include "logistics/Route.h"
#include "logistics/TransferRequest.h"
#include "finance/CashAccount.h"
#include "finance/Payment.h"
#include "finance/FinancialReport.h"
#include "management/AccessRole.h"
#include "management/AuditLog.h"
#include "management/KPIReport.h"
#include "management/MaintenanceRequest.h"
#include "management/StockAlert.h"
#include "management/WarehouseSystem.h"
#include "orders/Customer.h"
#include "orders/CustomerInvoice.h"
#include "orders/Shipment.h"
#include "finance/CostCalculation.h"
#include "orders/ReturnRequest.h"
#include <cassert>
#include <iostream>
int main(){
 Product p("SKU-1","Bolt",2.5,5,"FASTENERS","ACME"); p.addBarcode("4601234567890"); assert(p.needsReorder(4)); assert(p.isAvailableForSale(3,10)); p.changePrice(3.0); assert(p.unitPrice()==3.0);
 Warehouse w("WH-1","Main warehouse",100); StorageZone z("Z-1","Ambient",50,0.0); w.addZone(z); assert(w.containsZone("Z-1")); w.receive(70); assert(w.freeCapacity()==30);
 bool full=false;try{w.receive(40);}catch(const StorageCapacityExceededException&){full=true;}assert(full);w.dispatch(20);assert(w.occupied()==50);
 StockItem stock("ST-1","SKU-1","WH-1","BIN-1",20); StockReservation r("RS-1","SO-1","SKU-1",6);r.apply(stock);assert(stock.available()==14);r.consume(stock);assert(stock.onHand()==14&&stock.reserved()==0);
 SalesOrder order("SO-1","C-1");order.addLine(SalesOrderLine("SKU-1",3,2.5));assert(order.calculateTotal()==7.5);order.confirm();order.beginPicking();order.markShipped();assert(order.status()==SalesOrderStatus::Shipped);
 bool insufficient=false;try{stock.issue(100);}catch(const InsufficientStockException&){insufficient=true;}assert(insufficient);
 Inventory inventory("INV-1","WH-1"); inventory.addLine(InventoryLine("SKU-1",10)); inventory.addLine(InventoryLine("SKU-2",5)); inventory.start();
 inventory.recordCount("SKU-1",9); inventory.recordCount("SKU-2",5); assert(inventory.discrepancyCount()==1); assert(inventory.totalDifference()==-1); inventory.complete(); assert(inventory.status()==InventoryStatus::Completed);
 // Count independently, then verify discrepancy aggregation.
 InventoryLine lineA("SKU-A",8); lineA.recordCount(7); assert(lineA.difference()==-1);
 PurchaseOrder po("PO-1","SUP-1"); po.addLine(PurchaseOrderLine("SKU-1",10,2.0)); po.approve(); po.registerReceipt("SKU-1",4); assert(po.status()==PurchaseOrderStatus::PartiallyReceived); po.registerReceipt("SKU-1",6); assert(po.fullyReceived());

 // Personnel subsystem: employee profile, skills, shifts, task assignment and inspection.
 Employee employee("EMP-1", "Alex Smith", "Picker", 18.5);
 employee.addSkill("forklift");
 assert(employee.validateIdentity() && employee.hasSkill("forklift"));
 assert(employee.changePosition("Senior picker"));
 assert(employee.changeHourlyRate(20.0));

 Delivery delivery("DEL-1", "SHIP-1", 4, 0.0);
 Driver driver("EMP-2", "LIC-123", "2027-12-31", 1200.0);
 assert(driver.validateLicense("2026-11-20") && driver.canCarry(800.0));
 assert(driver.assignDelivery(delivery));
 assert(driver.recordDelivery("DEL-1") && driver.completedDeliveries() == 1);

 StockLot inspectedLot("LOT-1", "SKU-1", 12, std::time(nullptr) + 86400 * 30, 2.0);
 QualityInspector inspector("EMP-3", "QC-77", "2027-12-31");
 assert(inspector.certificateValidOn("2026-11-20"));
 assert(inspector.inspectLot(inspectedLot, false) && inspectedLot.quarantined());
 assert(inspector.inspectLot(inspectedLot, true) && !inspectedLot.quarantined());

 StorageZone assignedZone("Z-STORE", "Ambient", 100.0);
 PickingTask pickTask("PICK-1", "SO-1", 3, 0.0);
 Storekeeper storekeeper("EMP-4", "Z-STORE", 2);
 assert(storekeeper.assignZone(assignedZone));
 assert(storekeeper.assignPickingTask(pickTask));
 assert(storekeeper.countAssignedTasks() == 1);
 assert(storekeeper.completeTask("PICK-1"));

 WarehouseWorker worker("EMP-5", "DAY", 17.0);
 PackingTask packTask("PACK-1", "SO-1", 2, 0.0);
 assert(worker.startShift("2026-11-20"));
 assert(worker.assignPackingTask(packTask));
 assert(worker.completeTask("PACK-1"));
 assert(worker.calculateShiftPay(8.0) == 136.0);
 assert(worker.finishShift("2026-11-20"));

 // Procurement subsystem: request approval, supplier contract and goods receipt.
 PurchaseRequest request("PR-1", "OPERATIONS", "EMP-6");
 assert(request.addLine({"SKU-1", 10, 2.5}));
 assert(request.submit());
 WarehouseManager manager("EMP-M", "OPERATIONS", 1000.0, 4);
 assert(manager.approvePurchaseRequest(request));
 assert(request.status() == PurchaseRequestStatus::Approved);
 assert(request.approverId() == "EMP-M");

 Supplier supplier("SUP-1", "Acme Supplies", "orders@acme.example", 5, 4.0);
 supplier.recordOnTimeDelivery();
 assert(supplier.reliabilityScore() == 5.0);
 SupplierContract contract("CON-1", "SUP-1", "2026-01-01", "2026-12-31", 100.0, 0.01);
 assert(contract.isValidOn("2026-11-20") && contract.belongsTo(supplier));
 assert(contract.calculatePenalty(3, 500.0) == 15.0);

 PurchaseOrder receiptOrder("PO-RECEIPT", "SUP-1");
 receiptOrder.addLine(PurchaseOrderLine("SKU-1", 10, 2.0));
 receiptOrder.approve();
 DeliveryNote note("DN-1", "SUP-1", "PO-RECEIPT");
 assert(note.addLine({"SKU-1", 6}));
 assert(note.validateAgainstOrder(receiptOrder));
 GoodsReceipt receipt("GR-1", "PO-RECEIPT", "EMP-QC");
 assert(receipt.acceptDelivery(note, receiptOrder, "2026-11-20"));
 assert(receipt.completed() && receipt.acceptedQuantity() == 6);
 assert(receiptOrder.status() == PurchaseOrderStatus::PartiallyReceived);

 WarehouseExpense expense("EXP-1", "Equipment", 1, 250.0);
 assert(manager.approveExpense(expense));
 assert(expense.approvedBy() == "EMP-M");


 // Typed associations and domain operations added in the model expansion.
 ProductCategory category("FASTENERS", "Fasteners");
 Brand brand("ACME", "Acme");
 Barcode barcode("4601234567890", "EAN13");
 assert(category.containsProduct(p));
 assert(brand.matchesProduct(p));
 assert(p.registerBarcode(barcode));
 ProductBundle bundle("BUNDLE-1", "Bolt kit");
 assert(bundle.addComponent(p, 2));
 assert(bundle.calculateBundlePrice() == 6.0);
 SerialNumber serial("SN-1", "SKU-1");
 assert(serial.belongsTo(p));
 UnitOfMeasure unit("PCS", "pc", 0, 1.0);
 assert(unit.canMeasure(p));

 Rack rack("R-1", "Z-1");
 Shelf shelf("SH-1", "R-1");
 StorageBin bin("BIN-1", "SH-1");
 assert(rack.belongsTo(z));
 assert(shelf.belongsTo(rack));
 assert(bin.belongsTo(shelf));
 StorageCondition condition("TEMP", "temperature controlled", 0, 0.0);
 ColdStorage cold("COLD-1", "-5");
 assert(cold.accepts(condition) && cold.isBelowFreezing());
 LoadingDock dock("DOCK-1", "open");
 assert(dock.scheduleDelivery(delivery));
 assert(dock.isOccupied());

 assert(stock.matchesProduct(p));
 assert(inspectedLot.matchesProduct(p));
 StockBalance balance("BAL-1", "WH-1", 15, 0.0);
 assert(balance.belongsTo(w) && balance.hasAtLeast(10));
 StockMovement movement("MOV-1", "SKU-1", 3, 0.0);
 assert(movement.tracks(stock) && movement.isInbound());
 StockAdjustment adjustment("ADJ-1", "Cycle count", 2, 0.0);
 assert(adjustment.applyTo(balance, -2));
 assert(balance.quantity() == 13);
 ReorderRule reorder("RR-1", "SKU-1", 15, 0.0);
 assert(reorder.appliesTo(stock));
 assert(reorder.shortageAgainst(stock) == 1);
 assert(reorder.shouldReorder(stock));
 assert(inventory.belongsTo(w));
 InventoryLine reconciled("SKU-1", 14);
 assert(reconciled.reconcileWith(stock));
 assert(reconciled.counted() && reconciled.countedQuantity() == stock.onHand());

 assert(receiptOrder.suppliedBy(supplier));
 Customer customer("C-1", "Jane Customer");
 assert(customer.ownsOrder(order));
 assert(pickTask.belongsTo(order));
 assert(packTask.belongsTo(order));
 Shipment shipment("SHIP-1", "SO-1", 2, 0.0);
 assert(shipment.belongsTo(order));
 assert(delivery.refersTo(shipment));
 Carrier carrier("CARR-1", "Local Carrier", 0, 12.0);
 Transport van("VAN-1", "AB-123-CD", 10, 0.0);
 Route route("ROUTE-1", "Central depot", 0, 20.0);
 assert(carrier.canHandle(delivery));
 assert(van.canCarry(delivery));
 assert(route.supports(van));
 TransferRequest transfer("TR-1", "WH-1", 5, 10.0);
 Warehouse destination("WH-2", "Secondary warehouse", 100.0);
 assert(transfer.setDestination(destination));
 assert(transfer.connects(w, destination));

 CashAccount account("ACC-1", "GBP", 0, 500.0);
 Payment outgoing("PAY-1", "Supplier invoice", 1, 75.0);
 assert(outgoing.charge(account));
 assert(account.amount() == 425.0);
 assert(!outgoing.charge(account)); // повторное списание с того же счёта запрещено
 assert(account.amount() == 425.0);
 Payment incoming("PAY-2", "Customer payment", 1, 25.0);
 assert(account.recordPayment(incoming));
 assert(account.amount() == 450.0);
 FinancialReport financial("FR-1", "2026-11", 1, 450.0);
 assert(expense.attachTo(financial));
 assert(financial.includesExpense(expense));

 AuditLog audit("EV-1", "EMP-1");
 assert(audit.records(employee));
 AccessRole role("STOREKEEPER", "Storekeeper role", 4, 0.0);
 assert(role.assignTo(employee));
 MaintenanceRequest maintenance("MR-1", "FORKLIFT-7");
 assert(maintenance.assignWarehouse(w));
 assert(maintenance.warehouseCode() == w.code());
 StockAlert alert("AL-1", "SKU-1", 2, 0.0);
 assert(alert.follows(reorder));
 KPIReport kpi("KPI-1", "2026-11", 12, 30.0);
 assert(kpi.evaluates(inventory));
 WarehouseSystem system("SYS-1", "Central WMS");
 assert(system.registerWarehouse(w) && system.hasWarehouse("WH-1"));
 assert(role.hasPermissionCount(3));
 assert(alert.isCritical(3));
 assert(account.withdrawMoney(10.0));
 assert(account.creditMoney(5.0));
 assert(account.amount() == 445.0);


 // Additional boundary checks for the expanded behavior catalog.
 CostCalculation cost("COST-1", "SKU-1", 2, 6.0);
 assert(cost.calculateAverageCost() == 6.0);
 assert(cost.addPurchase(2, 4.0));
 assert(cost.calculateAverageCost() == 5.0);
 assert(outgoing.isRefundable());
 assert(expense.exceedsLimit(200.0));
 assert(financial.averageTransactionValue() == 450.0);
 assert(balance.calculateAvailable() == 13.0);
 assert(adjustment.isMaterial(2));
 assert(carrier.isCostEffective(15.0));
 assert(delivery.isOverdue(2, 3));
 assert(van.needsService(10));
 assert(route.isWithinDistanceLimit(25.0));
 customer.updateLoyalty(5);
 assert(customer.qualifiesForLoyaltyDiscount(5));
 CustomerInvoice invoice("INV-1", "C-1", 1, 200.0);
 assert(invoice.markPaid(150.0));
 assert(invoice.hasOutstandingBalance() && invoice.outstandingBalance() == 50.0);
 assert(invoice.markPaid(50.0));
 assert(!invoice.hasOutstandingBalance() && invoice.outstandingBalance() == 0.0);
 assert(pickTask.remainingQuantity() == 3);
 assert(pickTask.markPicked(1));
 assert(pickTask.pickedQuantity() == 1 && pickTask.remainingQuantity() == 2);
 assert(!pickTask.markPicked(3));
 PackingTask oversizeTask("PACK-2", "SO-1", 3, 100.0);
 assert(oversizeTask.requiresOversizePackaging(50.0));
 ReturnRequest returnRequest("RET-1", "SO-1", 3, 25.0);
 assert(returnRequest.isWithinQuantityLimit(5));
 assert(barcode.isNumeric());
 assert(brand.isPremium(0.0));
 ProductAttribute attribute("COLOR", "blue");
 assert(attribute.hasValue());
 ProductCategory populatedCategory("TOOLS", "Tools", 5, 0.0);
 assert(populatedCategory.isPopulated());
 assert(bundle.isAvailable(2));
 assert(serial.isRegistered());
 assert(unit.toBase(3.0) == 3.0);
 assert(cold.isBelowFreezing());
 assert(rack.canSupport(0.0));
 assert(shelf.hasCapacityFor(0.0));
 assert(bin.isEmpty());
 assert(condition.isTemperatureControlled());
 assert(audit.belongsToActor("EMP-1"));
 assert(maintenance.isHighPriority(0.0));
 assert(kpi.averageOrdersPerPeriod() == 0.4);
 assert(system.hasWarehouse("WH-1"));

 std::cout<<"Warehouse domain tests passed\n";
}
