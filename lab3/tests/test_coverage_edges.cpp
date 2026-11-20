#include "exceptions/AccessDeniedException.h"
#include "exceptions/DuplicateProductException.h"
#include "exceptions/ExpiredProductException.h"
#include "exceptions/InsufficientStockException.h"
#include "exceptions/InvalidOrderStateException.h"
#include "exceptions/InvalidStorageLocationException.h"
#include "exceptions/InventoryMismatchException.h"
#include "exceptions/PaymentException.h"
#include "exceptions/ProductNotFoundException.h"
#include "exceptions/PurchaseOrderException.h"
#include "exceptions/ReservationException.h"
#include "exceptions/ShipmentException.h"
#include "exceptions/StorageCapacityExceededException.h"
#include "exceptions/SupplierNotFoundException.h"
#include "exceptions/WarehouseException.h"
#include "inventory/StockReservation.h"
#include "inventory/StockItem.h"
#include "orders/SalesOrderLine.h"
#include "orders/SalesOrder.h"
#include "storage/StorageZone.h"
#include "storage/Warehouse.h"
#include "inventory/Inventory.h"
#include "inventory/InventoryLine.h"
#include "inventory/StockLot.h"
#include "products/Product.h"
#include "procurement/PurchaseOrderLine.h"
#include "procurement/PurchaseOrder.h"
#include "procurement/GoodsReceipt.h"
#include "procurement/DeliveryNote.h"
#include "people/WarehouseManager.h"
#include "procurement/PurchaseRequest.h"
#include "procurement/Supplier.h"
#include "finance/WarehouseExpense.h"
#include <cassert>
#include <functional>
#include <stdexcept>
#include <string>

static void expectException(const std::function<void()>& action) {
    bool thrown = false;
    try { action(); }
    catch (const std::exception&) { thrown = true; }
    assert(thrown);
}

int main() {
    // All custom exception constructors are instantiated and their message is checked.
    assert(std::string(AccessDeniedException("access").what()) == "access");
    assert(std::string(DuplicateProductException("duplicate").what()) == "duplicate");
    assert(std::string(ExpiredProductException("expired").what()) == "expired");
    assert(std::string(InsufficientStockException("stock").what()) == "stock");
    assert(std::string(InvalidOrderStateException("state").what()) == "state");
    assert(std::string(InvalidStorageLocationException("location").what()) == "location");
    assert(std::string(InventoryMismatchException("inventory").what()) == "inventory");
    assert(std::string(PaymentException("payment").what()) == "payment");
    assert(std::string(ProductNotFoundException("product").what()) == "product");
    assert(std::string(PurchaseOrderException("purchase").what()) == "purchase");
    assert(std::string(ReservationException("reservation").what()) == "reservation");
    assert(std::string(ShipmentException("shipment").what()) == "shipment");
    assert(std::string(StorageCapacityExceededException("capacity").what()) == "capacity");
    assert(std::string(SupplierNotFoundException("supplier").what()) == "supplier");
    assert(std::string(WarehouseException("warehouse").what()) == "warehouse");

    // Reservation release, invalid release, wrong SKU and consumed/released states.
    StockItem reservedStock("S-REL", "SKU-REL", "WH", "BIN", 10);
    StockReservation releaseReservation("R-REL", "O-REL", "SKU-REL", 3);
    releaseReservation.apply(reservedStock);
    assert(reservedStock.reserved() == 3);
    releaseReservation.release(reservedStock);
    assert(!releaseReservation.active());
    assert(releaseReservation.status() == ReservationStatus::Released);
    assert(reservedStock.reserved() == 0);
    expectException([&] { releaseReservation.release(reservedStock); });
    expectException([&] { releaseReservation.apply(reservedStock); });
    StockItem wrongStock("S-WRONG", "OTHER", "WH", "BIN", 10);
    StockReservation wrongReservation("R-WRONG", "O", "SKU-REL", 2);
    expectException([&] { wrongReservation.apply(wrongStock); });
    expectException([&] { StockReservation bad("", "O", "SKU", 1); });

    // Constructor and validation boundaries for storage and order lines.
    expectException([&] { StorageZone bad("", "ambient", 10, 0); });
    expectException([&] { StorageZone bad("Z", "ambient", 0, 0); });
    expectException([&] { StorageZone bad("Z", "ambient", 10, 11); });
    StorageZone zone("Z-EDGE", "ambient", 10, 5);
    expectException([&] { zone.store(0); });
    expectException([&] { zone.store(6); });
    expectException([&] { zone.remove(0); });
    expectException([&] { zone.remove(6); });
    zone.remove(5);
    zone.store(5);
    assert(zone.occupancyRate() == 0.5);

    SalesOrderLine line("SKU-EDGE", 2, 10.0);
    expectException([&] { line.changeQuantity(0); });
    expectException([&] { line.applyDiscount(-1); });
    expectException([&] { line.applyDiscount(101); });
    line.applyDiscount(100);
    assert(line.lineTotal() == 0.0);
    expectException([&] { SalesOrderLine bad("SKU", 0, 1); });
    expectException([&] { SalesOrderLine bad("SKU", 1, -1); });

    PurchaseOrderLine purchaseLine("SKU-EDGE", 5, 2.0);
    expectException([&] { purchaseLine.receive(0); });
    expectException([&] { purchaseLine.receive(6); });
    purchaseLine.receive(5);
    assert(purchaseLine.outstanding() == 0);

    // Inventory state transitions and missing/duplicate line errors.
    Inventory emptyInventory("INV-EMPTY", "WH");
    expectException([&] { emptyInventory.start(); });
    Inventory inv("INV-EDGE", "WH");
    InventoryLine il("SKU-EDGE", 5);
    inv.addLine(il);
    expectException([&] { inv.addLine(il); });
    expectException([&] { inv.totalDifference(); });
    inv.start();
    expectException([&] { inv.addLine(InventoryLine("SKU-X", 1)); });
    expectException([&] { inv.recordCount("MISSING", 1); });
    expectException([&] { inv.complete(); });
    inv.recordCount("SKU-EDGE", 4);
    inv.complete();
    expectException([&] { inv.cancel(); });

    // Sales order invalid state transitions and duplicate/missing lines.
    SalesOrder order("SO-EDGE", "C-EDGE");
    expectException([&] { order.confirm(); });
    order.addLine(SalesOrderLine("SKU-1", 1, 1.0));
    expectException([&] { order.addLine(SalesOrderLine("SKU-1", 2, 1.0)); });
    expectException([&] { order.removeLine("MISSING"); });
    order.confirm();
    expectException([&] { order.addLine(SalesOrderLine("SKU-2", 1, 1.0)); });
    order.beginPicking();
    order.markShipped();
    expectException([&] { order.cancel(); });

    // Stock lot: normal issue, quarantine, expired and insufficient branches.
    const auto now = std::time(nullptr);
    StockLot lot("LOT-EDGE", "SKU-EDGE", 5, now + 86400, 2.0);
    lot.remove(2);
    assert(lot.quantity() == 3);
    expectException([&] { lot.remove(4); });
    lot.markQuarantined();
    expectException([&] { lot.remove(1); });
    lot.releaseFromQuarantine();
    StockLot expired("LOT-OLD", "SKU-EDGE", 2, now - 1, 1.0);
    expectException([&] { expired.remove(1); });

    // StockItem validation and every mutating guard.
    expectException([&] { StockItem bad("", "SKU", "WH", "BIN", 1); });
    expectException([&] { StockItem bad("S", "SKU", "WH", "BIN", 1, 2); });
    StockItem item("S-EDGE", "SKU", "WH", "BIN", 5);
    expectException([&] { item.receive(0); });
    expectException([&] { item.reserve(0); });
    expectException([&] { item.reserve(6); });
    item.reserve(2);
    expectException([&] { item.release(3); });
    item.release(1);
    expectException([&] { item.issue(5); });
    item.issue(1);
    expectException([&] { item.moveTo(""); });
    item.moveTo("BIN-2");
    item.receive(2);
    assert(item.binCode() == "BIN-2" && item.onHand() == 6);

    // Warehouse closed/full/invalid-volume paths and duplicate zone registration.
    Warehouse warehouse("WH-EDGE", "Address", 10);
    warehouse.addZone(zone);
    warehouse.addZone(zone);
    assert(warehouse.zoneCodes().size() == 1);
    expectException([&] { warehouse.receive(0); });
    warehouse.receive(10);
    expectException([&] { warehouse.receive(1); });
    expectException([&] { warehouse.dispatch(11); });
    warehouse.dispatch(10);
    warehouse.close();
    expectException([&] { warehouse.receive(1); });
    warehouse.reopen();
    warehouse.receive(1);
    assert(warehouse.occupancyRate() == 0.1);

    // Product validation, duplicate barcodes and inactive-product checks.
    expectException([&] { Product bad("", "Name", 1, 0, "C", "B"); });
    expectException([&] { Product bad("SKU", "Name", -1, 0, "C", "B"); });
    Product product("SKU-P", "Product", 2.0, 3, "CAT", "BR");
    expectException([&] { product.changePrice(-1); });
    expectException([&] { product.addBarcode(""); });
    product.addBarcode("123");
    expectException([&] { product.addBarcode("123"); });
    assert(product.isAvailableForSale(1, 1));
    assert(!product.isAvailableForSale(0, 10));
    product.deactivate();
    assert(!product.needsReorder(0));
    product.activate();

    // Supplier rating limits, delivery statistics and deactivation.
    expectException([&] { Supplier bad("", "Company", "mail", 1, 4); });
    Supplier supplier("SUP-EDGE", "Company", "mail", 2, 4.0);
    assert(!supplier.rateReliability(-1));
    assert(!supplier.rateReliability(6));
    assert(supplier.rateReliability(3.5));
    assert(supplier.reliabilityScore() == 3.5);
    supplier.recordLateDelivery();
    supplier.recordOnTimeDelivery();
    assert(supplier.reliabilityScore() == 2.5);
    assert(supplier.deactivate());
    assert(!supplier.deactivate());

    // Purchase request rejection, duplicate line aggregation and terminal states.
    PurchaseRequest request("PR-EDGE", "OPS", "EMP");
    assert(!request.addLine({"", 1, 1.0}));
    assert(!request.addLine({"SKU", 0, 1.0}));
    assert(request.addLine({"SKU", 2, 3.0}));
    assert(request.addLine({"SKU", 1, 4.0}));
    assert(request.lines().size() == 1 && request.lines()[0].quantity == 3);
    assert(request.hasLine("SKU") && !request.hasLine("NO"));
    assert(request.estimatedCost() == 12.0);
    assert(request.submit());
    assert(!request.submit());
    assert(!request.approve(""));
    assert(request.reject("MGR", "not required"));
    assert(request.cancel());
    assert(!request.cancel());

    PurchaseRequest approvedRequest("PR-APP", "OPS", "EMP");
    assert(approvedRequest.addLine({"SKU", 1, 1.0}));
    assert(approvedRequest.submit());
    assert(approvedRequest.approve("MGR"));
    assert(!approvedRequest.cancel());
    WarehouseManager manager("MGR", "OPS", 5.0, 0);
    assert(manager.canApprove(5.0) && !manager.canApprove(6.0));
    assert(manager.calculateTeamLoad(10.0) == 0.0);
    manager.registerTeamMember("MGR");
    manager.registerTeamMember("");
    manager.registerTeamMember("EMP-1");
    manager.registerTeamMember("EMP-1");
    assert(manager.calculateTeamLoad(10.0) == 10.0);
    PurchaseRequest expensive("PR-EXP", "OPS", "EMP");
    assert(expensive.addLine({"SKU", 10, 2.0}) && expensive.submit());
    assert(!manager.approvePurchaseRequest(expensive));
    WarehouseExpense expense("EXP-EDGE", "misc", 1, 10.0);
    assert(!manager.approveExpense(expense));

    // Goods receipt invalid inputs, rejected lines and accepted delivery.
    PurchaseOrder po("PO-GR", "SUP");
    po.addLine(PurchaseOrderLine("SKU-GR", 5, 2.0));
    po.approve();
    DeliveryNote note("DN-GR", "SUP", "PO-GR");
    assert(note.addLine({"SKU-GR", 3}));
    GoodsReceipt receipt("GR-EDGE", "PO-GR", "QC");
    assert(!receipt.acceptDelivery(note, po, "bad-date"));
    assert(receipt.rejectLine("SKU-GR", 1, "damaged"));
    assert(!receipt.rejectLine("", 1, "reason"));
    assert(receipt.acceptDelivery(note, po, "2026-11-20"));
    assert(!receipt.acceptDelivery(note, po, "2026-11-20"));
    assert(!receipt.rejectLine("SKU-GR", 1, "late"));
    assert(receipt.acceptedQuantity() == 3 && receipt.rejectedQuantity() == 1);
    assert(receipt.calculateAcceptedValue(po) == 6.0);
}
