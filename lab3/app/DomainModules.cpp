#include "DomainModules.h"

#include "finance/CashAccount.h"
#include "finance/CostCalculation.h"
#include "finance/FinancialReport.h"
#include "finance/Payment.h"
#include "finance/WarehouseExpense.h"
#include "inventory/Inventory.h"
#include "inventory/InventoryLine.h"
#include "inventory/ReorderRule.h"
#include "inventory/StockAdjustment.h"
#include "inventory/StockBalance.h"
#include "inventory/StockItem.h"
#include "inventory/StockLot.h"
#include "inventory/StockMovement.h"
#include "inventory/StockReservation.h"
#include "logistics/Carrier.h"
#include "logistics/Delivery.h"
#include "logistics/Route.h"
#include "logistics/TransferRequest.h"
#include "logistics/Transport.h"
#include "management/AccessRole.h"
#include "management/AuditLog.h"
#include "management/KPIReport.h"
#include "management/MaintenanceRequest.h"
#include "management/StockAlert.h"
#include "management/WarehouseSystem.h"
#include "orders/Customer.h"
#include "orders/CustomerInvoice.h"
#include "orders/PackingTask.h"
#include "orders/PickingTask.h"
#include "orders/ReturnRequest.h"
#include "orders/SalesOrder.h"
#include "orders/SalesOrderLine.h"
#include "orders/Shipment.h"
#include "people/Driver.h"
#include "people/Employee.h"
#include "people/QualityInspector.h"
#include "people/Storekeeper.h"
#include "people/WarehouseManager.h"
#include "people/WarehouseWorker.h"
#include "products/Barcode.h"
#include "products/Brand.h"
#include "products/Product.h"
#include "products/ProductAttribute.h"
#include "products/ProductBundle.h"
#include "products/ProductCategory.h"
#include "products/SerialNumber.h"
#include "products/UnitOfMeasure.h"
#include "storage/ColdStorage.h"
#include "storage/LoadingDock.h"
#include "storage/Rack.h"
#include "storage/Shelf.h"
#include "storage/StorageBin.h"
#include "storage/StorageCondition.h"
#include "storage/StorageZone.h"
#include "storage/Warehouse.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

namespace {
void section(const char* title) {
    std::cout << "\n\n========== " << title << " ==========\n";
}

std::time_t futureDate() {
    return std::time(nullptr) + 365 * 24 * 60 * 60;
}

std::string currentIsoDate() {
    const std::time_t now = std::time(nullptr);
    std::tm localTime{};
#if defined(_WIN32)
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif
    char buffer[11]{};
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localTime);
    return buffer;
}
}

void runExtendedDomainDemonstration() {
    std::cout << "\nThis demonstration exercises the remaining warehouse subsystems.\n";
    std::cout << "The objects below are sample in-memory records; they do not replace\n"
                 "the persistent records managed by the main application.\n";

    // Shared base entities used by multiple subsystems.
    Warehouse warehouse("WH-DEMO", "Demonstration warehouse", 5000.0);
    StorageZone zone("ZONE-DEMO", "Ambient", 3000.0);
    warehouse.addZone(zone);
    Product product("SKU-DEMO", "Demonstration filter", 25.0, 5, "CAT-DEMO", "BR-DEMO");
    StockItem stock("ST-DEMO", product.sku(), warehouse.code(), "BIN-DEMO", 40);

    section("PRODUCT CATALOG");
    Brand brand("BR-DEMO", "Northwind Industrial", 1, 150.0);
    ProductCategory category("CAT-DEMO", "Filters", 1, 0.0);
    Barcode barcode("1234567890123", "EAN-13");
    ProductAttribute attribute("MATERIAL", "Stainless steel");
    ProductBundle bundle("BUNDLE-01", "Maintenance kit");
    SerialNumber serial("SER-0001", product.sku());
    UnitOfMeasure unit("PCS", "pc", 1, 1.0);

    product.registerBarcode(barcode);
    const bool brandMatches = brand.matchesProduct(product);
    const bool categoryMatches = category.containsProduct(product);
    const bool bundleCreated = bundle.addComponent(product, 2);
    const bool serialMatches = serial.belongsTo(product);
    const bool unitMatches = unit.canMeasure(product);
    attribute.validateValue();

    std::cout << "Product: " << product.sku() << " - " << product.name() << '\n'
              << "Brand match: " << std::boolalpha << brandMatches
              << ", category match: " << categoryMatches
              << ", barcode valid: " << barcode.validateBarcode()
              << ", attribute valid: " << attribute.validateValue()
              << ", bundle created: " << bundleCreated
              << ", serial assigned to product: " << serialMatches
              << ", unit compatible: " << unitMatches << '\n'
              << "Bundle price: " << bundle.calculateBundlePrice()
              << ", 3 units in base measure: " << unit.toBase(3.0) << '\n';

    section("WAREHOUSE LAYOUT AND STORAGE CONDITIONS");
    StorageCondition ambientCondition("COND-AMBIENT", "Dry ambient storage", 1, 0.0);
    ColdStorage coldStorage("COLD-01", "-18 C", 0, 12.0);
    Rack rack("RACK-01", zone.code(), 0, 500.0);
    Shelf shelf("SHELF-01", rack.rackCode(), 0, 100.0);
    StorageBin bin("BIN-01", shelf.shelfCode(), 0, 50.0);
    LoadingDock dock("DOCK-01", "open");

    rack.hasFreeLevel(1);
    shelf.canStore(1);
    bin.canFit(1);
    coldStorage.validateTemperature();

    std::cout << "Zone: " << zone.code()
              << ", rack belongs to zone: " << rack.belongsTo(zone)
              << ", shelf belongs to rack: " << shelf.belongsTo(rack)
              << ", bin belongs to shelf: " << bin.belongsTo(shelf)
              << ", cold storage accepts condition: " << coldStorage.accepts(ambientCondition)
              << ", dock open: " << dock.openDock() << '\n';

    section("INVENTORY AND STOCK CONTROL");
    Inventory inventory("INV-DEMO", warehouse.code());
    InventoryLine inventoryLine(product.sku(), stock.onHand());
    inventory.addLine(inventoryLine);
    inventory.start();
    inventory.recordCount(product.sku(), stock.onHand());
    const bool reconciled = inventoryLine.reconcileWith(stock);
    inventory.complete();

    StockBalance balance("BAL-DEMO", warehouse.code(), stock.onHand(), 1000.0);
    StockAdjustment adjustment("ADJ-DEMO", "Cycle count correction", 1, 25.0);
    const bool adjustmentApplied = adjustment.applyTo(balance, 1);
    StockMovement movement("MOV-DEMO", product.sku(), 1, 25.0);
    StockLot demoLot("LOT-DEMO", product.sku(), 4, futureDate(), product.unitPrice());
    QualityInspector demoInspector("QA-DEMO", "CERT-DEMO", "2099-12-31");
    demoInspector.inspectLot(demoLot, true);
    ReorderRule reorderRule("REORDER-DEMO", product.sku(), 5, 0.0);
    StockAlert stockAlert("ALERT-DEMO", product.sku(), stock.available(), 0.0);
    StockReservation reservation("RES-DEMO", "SO-DEMO", product.sku(), 2);
    reservation.apply(stock);
    reservation.release(stock);

    std::cout << "Inventory: " << inventory.id()
              << ", completed: " << (inventory.status() == InventoryStatus::Completed)
              << ", discrepancy count: " << inventory.discrepancyCount()
              << ", line reconciled: " << reconciled << '\n'
              << "Balance belongs to warehouse: " << balance.belongsTo(warehouse)
              << ", adjustment applied: " << adjustmentApplied
              << ", movement tracks stock: " << movement.tracks(stock)
              << ", demo lot passed inspection: " << !demoLot.quarantined()
              << ", reorder required: " << reorderRule.shouldReorder(stock)
              << ", alert linked to rule: " << stockAlert.follows(reorderRule)
              << ", reservation released: " << (reservation.status() == ReservationStatus::Released)
              << '\n';

    section("CUSTOMERS, ORDERS, PICKING, PACKING AND RETURNS");
    Customer customer("CUST-DEMO", "Alex Morgan", 2, 0.0);
    SalesOrder salesOrder("SO-DEMO", customer.customerId());
    salesOrder.addLine(SalesOrderLine(product.sku(), 3, product.unitPrice()));
    salesOrder.confirm();

    PickingTask pickingTask("PICK-DEMO", salesOrder.orderId(), 3, 0.0);
    PackingTask packingTask("PACK-DEMO", salesOrder.orderId(), 3, 0.0);
    pickingTask.assignPicker();
    pickingTask.markPicked(3);
    packingTask.completePacking(3);

    Shipment shipment("SHIP-DEMO", salesOrder.orderId(), 3, 15.0);
    shipment.dispatch();
    ReturnRequest returnRequest("RET-DEMO", salesOrder.orderId(), 1, 25.0);
    const bool returnApproved = returnRequest.approveReturn();
    returnRequest.calculateRefund(1);

    std::cout << "Customer: " << customer.fullName()
              << ", owns order: " << customer.ownsOrder(salesOrder)
              << ", loyalty eligible: " << customer.qualifiesForLoyaltyDiscount(1) << '\n'
              << "Picking complete: " << (pickingTask.remainingQuantity() == 0)
              << ", packing belongs to order: " << packingTask.belongsTo(salesOrder)
              << ", shipment belongs to order: " << shipment.belongsTo(salesOrder)
              << ", return approved: " << returnApproved
              << ", refund amount: " << returnRequest.amount() << '\n';

    section("LOGISTICS AND INTER-WAREHOUSE TRANSFER");
    Delivery delivery("DEL-DEMO", shipment.shipmentId(), 3, 35.0);
    Carrier carrier("CARRIER-DEMO", "Northwind Logistics", 1, 35.0);
    Transport transport("TRUCK-DEMO", "DEMO-001", 10, 1000.0);
    Route route("ROUTE-DEMO", "Central distribution point", 1, 120.0);
    Driver driver("DRV-DEMO", "LIC-DEMO", "2099-12-31", 1500.0);
    LoadingDock outboundDock("DOCK-OUT", "open");
    Warehouse destination("WH-DEST", "Destination warehouse", 2000.0);
    TransferRequest transfer("TRANSFER-DEMO", warehouse.code(), 5, 50.0);

    const bool deliveryScheduled = outboundDock.scheduleDelivery(delivery);
    const bool driverAssigned = driver.assignDelivery(delivery);
    const bool routeSupportsTransport = route.supports(transport);
    const bool carrierCanHandle = carrier.canHandle(delivery);
    const bool transportCanCarry = transport.canCarry(delivery);
    const bool transferDestinationSet = transfer.setDestination(destination);

    std::cout << "Delivery confirmed: " << delivery.confirmDelivery()
              << ", carrier can handle: " << carrierCanHandle
              << ", transport can carry: " << transportCanCarry
              << ", carrier cost: " << carrier.calculateDeliveryCost()
              << ", route distance: " << route.calculateDistance()
              << ", route supports transport: " << routeSupportsTransport
              << ", driver assigned: " << driverAssigned
              << ", dock scheduled: " << deliveryScheduled << '\n'
              << "Transfer approved: " << transfer.approveTransfer()
              << ", destination set: " << transferDestinationSet
              << ", warehouses connected: " << transfer.connects(warehouse, destination)
              << '\n';

    section("EMPLOYEES AND ACCESS CONTROL");
    Employee employee("EMP-DEMO", "Jordan Lee", "Warehouse operator", 18.0);
    employee.addSkill("Inventory counting");
    WarehouseWorker worker("WORKER-DEMO", "DAY", 18.0);
    Storekeeper storekeeper("KEEPER-DEMO", zone.code(), 5);
    WarehouseManager manager("MANAGER-DEMO", "OPS", 5000.0, 4);
    AccessRole role("ROLE-STOCK", "Stock operator", 4, 0.0);
    AuditLog audit("AUDIT-DEMO", employee.employeeId(), 1, 0.0);

    worker.startShift(currentIsoDate());
    worker.assignPickingTask(pickingTask);
    storekeeper.assignZone(zone);
    storekeeper.assignPickingTask(pickingTask);
    manager.registerTeamMember(employee.employeeId());
    role.assignTo(employee);

    std::cout << "Employee valid: " << employee.validateIdentity()
              << ", skill registered: " << employee.hasSkill("Inventory counting")
              << ", worker on shift: " << worker.onShift()
              << ", storekeeper accepted task: " << (storekeeper.countAssignedTasks() > 0)
              << ", manager team load: " << manager.calculateTeamLoad(8.0)
              << ", role assigned: " << role.hasRelatedId(employee.employeeId())
              << ", audit records employee: " << audit.records(employee)
              << '\n';

    section("FINANCE AND PAYMENTS");
    CashAccount account("CASH-DEMO", "GBP", 0, 1000.0);
    Payment payment("PAY-DEMO", "Customer invoice", 1, 75.0);
    const bool paymentCharged = payment.charge(account);
    CostCalculation cost("COST-DEMO", product.sku(), 0, 0.0);
    cost.addPurchase(10, 20.0);
    WarehouseExpense expense("EXP-DEMO", "Equipment maintenance", 1, 120.0);
    const bool expenseApproved = manager.approveExpense(expense);
    FinancialReport financialReport("FIN-DEMO", "2026-11", 1, 75.0);
    expense.attachTo(financialReport);
    CustomerInvoice invoice("INV-CUST-DEMO", customer.customerId(), 1, 75.0);
    invoice.markPaid(75.0);

    std::cout << "Payment charged: " << paymentCharged
              << ", cash account balance: " << account.amount()
              << ", average product cost: " << cost.calculateAverageCost()
              << ", expense approved: " << expenseApproved
              << ", approved by: " << expense.approvedBy()
              << ", report includes expense: " << financialReport.includesExpense(expense)
              << ", invoice outstanding: " << invoice.outstandingBalance()
              << '\n';

    section("MANAGEMENT, REPORTING AND MAINTENANCE");
    WarehouseSystem system("WMS-DEMO", "Warehouse network", 0, 0.0);
    MaintenanceRequest maintenance("MAINT-DEMO", "FORKLIFT-01", 1, 800.0);
    KPIReport kpi("KPI-DEMO", "2026-11", 10, 2.0);

    system.registerWarehouse(warehouse);
    system.registerWarehouse(destination);
    system.calculateNetworkStock(stock.onHand());
    maintenance.assignWarehouse(warehouse);
    maintenance.scheduleMaintenance();
    kpi.calculateInventoryTurnover(1);

    std::cout << "Registered warehouses: " << system.relatedCount()
              << ", network stock: " << system.quantity()
              << ", maintenance assigned: " << maintenance.warehouseCode()
              << ", maintenance high priority: " << maintenance.isHighPriority(500.0)
              << ", KPI evaluates inventory: " << kpi.evaluates(inventory)
              << ", average orders per period: " << kpi.averageOrdersPerPeriod()
              << '\n';

    section("DEMONSTRATION COMPLETE");
    std::cout << "All domain groups have been exercised: products, storage, inventory, "
                 "orders, people, logistics, finance and management.\n";
}
