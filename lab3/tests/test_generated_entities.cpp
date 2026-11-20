#include "finance/CashAccount.h"
#include "finance/CostCalculation.h"
#include "finance/FinancialReport.h"
#include "finance/Payment.h"
#include "finance/WarehouseExpense.h"
#include "inventory/ReorderRule.h"
#include "inventory/StockAdjustment.h"
#include "inventory/StockBalance.h"
#include "inventory/StockMovement.h"
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
#include "orders/Shipment.h"
#include "products/Barcode.h"
#include "products/Brand.h"
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
#include <cassert>
#include <string>
#include "exceptions/WarehouseException.h"

void test_generated_entities() {
  { CashAccount o("TST-CashAccount", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.deposit();
    (void)o.withdraw(1);
    (void)o.withdraw(-1);
    (void)o.credit(1);
    (void)o.credit(-1);
    (void)o.creditMoney(1.0);
    (void)o.creditMoney(-1.0);
    (void)o.withdrawMoney(1.0);
    (void)o.withdrawMoney(-1.0);
  }
  { CostCalculation o("TST-CostCalculation", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.calculateAverageCost();
    (void)o.calculateWeightedCost(1);
    (void)o.calculateWeightedCost(-1);
  }
  { FinancialReport o("TST-FinancialReport", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.calculateNetResult();
    (void)o.validatePeriod(1);
    (void)o.validatePeriod(-1);
    (void)o.averageTransactionValue();
  }
  { Payment o("TST-Payment", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.processPayment();
    (void)o.refund(1);
    (void)o.refund(-1);
    (void)o.isRefundable();
  }
  { WarehouseExpense o("TST-WarehouseExpense", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.approveExpense();
    (void)o.isWithinBudget(1);
    (void)o.isWithinBudget(-1);
    (void)o.exceedsLimit(1.0);
    (void)o.exceedsLimit(-1.0);
  }
  { ReorderRule o("TST-ReorderRule", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.shouldReorder();
    (void)o.calculateOrderQuantity(1);
    (void)o.calculateOrderQuantity(-1);
  }
  { StockAdjustment o("TST-StockAdjustment", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.applyAdjustment();
    (void)o.requiresApproval(1);
    (void)o.requiresApproval(-1);
    (void)o.isMaterial(1);
    (void)o.isMaterial(-1);
  }
  { StockBalance o("TST-StockBalance", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.calculateAvailable();
    (void)o.reconcile(1);
    (void)o.reconcile(-1);
    (void)o.hasAtLeast(1);
    (void)o.hasAtLeast(-1);
  }
  { StockMovement o("TST-StockMovement", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.calculateSignedQuantity();
    (void)o.isTransfer(1);
    (void)o.isTransfer(-1);
    (void)o.isInbound();
  }
  { Carrier o("TST-Carrier", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.calculateDeliveryCost();
    (void)o.isContractActive(1);
    (void)o.isContractActive(-1);
    (void)o.isCostEffective(1.0);
    (void)o.isCostEffective(-1.0);
  }
  { Delivery o("TST-Delivery", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.confirmDelivery();
    (void)o.recordFailure(1);
    (void)o.recordFailure(-1);
  }
  { Route o("TST-Route", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.calculateDistance();
    (void)o.estimateDuration(1);
    (void)o.estimateDuration(-1);
    (void)o.isWithinDistanceLimit(1.0);
    (void)o.isWithinDistanceLimit(-1.0);
  }
  { TransferRequest o("TST-TransferRequest", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.approveTransfer();
    (void)o.calculateTransferCost(1);
    (void)o.calculateTransferCost(-1);
    (void)o.destinationWarehouse();
  }
  { Transport o("TST-Transport", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.isAvailable();
    (void)o.calculateCapacity(1);
    (void)o.calculateCapacity(-1);
    (void)o.needsService(1);
    (void)o.needsService(-1);
  }
  { AccessRole o("TST-AccessRole", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.grantPermission();
    (void)o.revokePermission(1);
    (void)o.revokePermission(-1);
    (void)o.hasPermissionCount(1);
    (void)o.hasPermissionCount(-1);
  }
  { AuditLog o("TST-AuditLog", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.recordEvent();
    (void)o.countEventsByActor(1);
    (void)o.countEventsByActor(-1);
  }
  { KPIReport o("TST-KPIReport", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.calculateOrderFulfillmentRate();
    (void)o.calculateInventoryTurnover(1);
    (void)o.calculateInventoryTurnover(-1);
    (void)o.averageOrdersPerPeriod();
  }
  { MaintenanceRequest o("TST-MaintenanceRequest", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.scheduleMaintenance();
    (void)o.closeRequest(1);
    (void)o.closeRequest(-1);
    (void)o.requestId();
    (void)o.equipmentCode();
    (void)o.isHighPriority(1.0);
    (void)o.isHighPriority(-1.0);
  }
  { StockAlert o("TST-StockAlert", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.triggerIfLow();
    (void)o.acknowledge(1);
    (void)o.acknowledge(-1);
    (void)o.isCritical(1);
    (void)o.isCritical(-1);
  }
  { WarehouseSystem o("TST-WarehouseSystem", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.registerWarehouse();
    (void)o.calculateNetworkStock(1);
    (void)o.calculateNetworkStock(-1);
    (void)o.systemName();
  }
  { Customer o("TST-Customer", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.validateContact();
    (void)o.updateLoyalty(1);
    (void)o.updateLoyalty(-1);
    (void)o.fullName();
    (void)o.qualifiesForLoyaltyDiscount(1);
    (void)o.qualifiesForLoyaltyDiscount(-1);
  }
  { CustomerInvoice o("TST-CustomerInvoice", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.calculateDue();
    (void)o.markPaid(1.0);
    (void)o.markPaid(-1.0);
    (void)o.hasOutstandingBalance();
    (void)o.outstandingBalance();
  }
  { PackingTask o("TST-PackingTask", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.calculatePackageCount();
    (void)o.completePacking(1);
    (void)o.completePacking(-1);
    (void)o.requiresOversizePackaging(1.0);
    (void)o.requiresOversizePackaging(-1.0);
  }
  { PickingTask o("TST-PickingTask", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.assignPicker();
    (void)o.markPicked(1);
    (void)o.markPicked(-1);
    (void)o.remainingQuantity();
    (void)o.pickedQuantity();
  }
  { ReturnRequest o("TST-ReturnRequest", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.approveReturn();
    (void)o.calculateRefund(1);
    (void)o.calculateRefund(-1);
    (void)o.isWithinQuantityLimit(1);
    (void)o.isWithinQuantityLimit(-1);
  }
  { Shipment o("TST-Shipment", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.dispatch();
    (void)o.markDelivered(1);
    (void)o.markDelivered(-1);
  }
  { Barcode o("TST-Barcode", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.validateBarcode();
    (void)o.matches(1);
    (void)o.matches(-1);
    (void)o.isNumeric();
  }
  { Brand o("TST-Brand", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.renameBrand();
    (void)o.matchesProduct(1);
    (void)o.matchesProduct(-1);
    (void)o.isPremium(1.0);
    (void)o.isPremium(-1.0);
  }
  { ProductAttribute o("TST-ProductAttribute", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.validateValue();
    (void)o.normalizeValue(1);
    (void)o.normalizeValue(-1);
    (void)o.hasValue();
  }
  { ProductBundle o("TST-ProductBundle", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.calculateBundlePrice();
    (void)o.validateComponents(1);
    (void)o.validateComponents(-1);
    (void)o.isAvailable(1);
    (void)o.isAvailable(-1);
  }
  { ProductCategory o("TST-ProductCategory", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.addProduct();
    (void)o.removeProduct(1);
    (void)o.removeProduct(-1);
    (void)o.isPopulated();
  }
  { SerialNumber o("TST-SerialNumber", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.assignProduct();
    (void)o.isAssigned(1);
    (void)o.isAssigned(-1);
    (void)o.isRegistered();
  }
  { UnitOfMeasure o("TST-UnitOfMeasure", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.convertToBase();
    (void)o.isCompatible(1);
    (void)o.isCompatible(-1);
    (void)o.toBase(1.0);
    (void)o.toBase(-1.0);
  }
  { ColdStorage o("TST-ColdStorage", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.validateTemperature();
    (void)o.estimateEnergyUse(1);
    (void)o.estimateEnergyUse(-1);
    (void)o.isBelowFreezing();
  }
  { LoadingDock o("TST-LoadingDock", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.openDock();
    (void)o.closeDock(1);
    (void)o.closeDock(-1);
    (void)o.isOccupied();
  }
  { Rack o("TST-Rack", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.calculateLoad();
    (void)o.hasFreeLevel(1);
    (void)o.hasFreeLevel(-1);
    (void)o.canSupport(1.0);
    (void)o.canSupport(-1.0);
  }
  { Shelf o("TST-Shelf", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.calculateFreeVolume();
    (void)o.canStore(1);
    (void)o.canStore(-1);
    (void)o.hasCapacityFor(1.0);
    (void)o.hasCapacityFor(-1.0);
  }
  { StorageBin o("TST-StorageBin", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.calculateFreeCapacity();
    (void)o.canFit(1);
    (void)o.canFit(-1);
    (void)o.isEmpty();
  }
  { StorageCondition o("TST-StorageCondition", "value", 2, 3.0);
    (void)o.active();
    (void)o.quantity();
    (void)o.amount();
    (void)o.relatedCount();
    o.addRelatedId("REL-1");
    o.addRelatedId("REL-1");
    o.addRelatedId("");
    assert(o.hasRelatedId("REL-1"));
    assert(!o.hasRelatedId("missing"));
    (void)o.isSatisfiedBy();
    (void)o.requiresCooling(1);
    (void)o.requiresCooling(-1);
    (void)o.isTemperatureControlled();
  }
}

void test_entity_getters_and_invalid_constructors();

int main() {
    test_generated_entities();
    test_entity_getters_and_invalid_constructors();
    return 0;
}

void test_entity_getters_and_invalid_constructors() {
  { CashAccount o("GET-CashAccount", "value", 2, 3.0);
    (void)o.deposit();
    (void)o.accountId();
    (void)o.currency();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { CashAccount bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { CostCalculation o("GET-CostCalculation", "value", 2, 3.0);
    (void)o.calculateAverageCost();
    (void)o.calculationId();
    (void)o.productSku();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { CostCalculation bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { FinancialReport o("GET-FinancialReport", "value", 2, 3.0);
    (void)o.calculateNetResult();
    (void)o.reportId();
    (void)o.period();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.averageTransactionValue();
    bool rejected = false;
    try { FinancialReport bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { Payment o("GET-Payment", "value", 2, 3.0);
    (void)o.processPayment();
    (void)o.paymentId();
    (void)o.purpose();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.isRefundable();
    bool rejected = false;
    try { Payment bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { WarehouseExpense o("GET-WarehouseExpense", "value", 2, 3.0);
    (void)o.approveExpense();
    (void)o.approvedBy();
    (void)o.expenseId();
    (void)o.category();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { WarehouseExpense bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { ReorderRule o("GET-ReorderRule", "value", 2, 3.0);
    (void)o.shouldReorder();
    (void)o.ruleId();
    (void)o.productSku();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { ReorderRule bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { StockAdjustment o("GET-StockAdjustment", "value", 2, 3.0);
    (void)o.applyAdjustment();
    (void)o.adjustmentId();
    (void)o.reason();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { StockAdjustment bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { StockBalance o("GET-StockBalance", "value", 2, 3.0);
    (void)o.calculateAvailable();
    (void)o.balanceId();
    (void)o.warehouseCode();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { StockBalance bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { StockMovement o("GET-StockMovement", "value", 2, 3.0);
    (void)o.calculateSignedQuantity();
    (void)o.movementId();
    (void)o.productSku();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.isInbound();
    bool rejected = false;
    try { StockMovement bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { Carrier o("GET-Carrier", "value", 2, 3.0);
    (void)o.calculateDeliveryCost();
    (void)o.carrierId();
    (void)o.companyName();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { Carrier bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { Delivery o("GET-Delivery", "value", 2, 3.0);
    (void)o.confirmDelivery();
    (void)o.deliveryId();
    (void)o.shipmentId();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { Delivery bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { Route o("GET-Route", "value", 2, 3.0);
    (void)o.calculateDistance();
    (void)o.routeId();
    (void)o.destination();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { Route bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { TransferRequest o("GET-TransferRequest", "value", 2, 3.0);
    (void)o.approveTransfer();
    (void)o.transferId();
    (void)o.sourceWarehouse();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.destinationWarehouse();
    bool rejected = false;
    try { TransferRequest bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { Transport o("GET-Transport", "value", 2, 3.0);
    (void)o.isAvailable();
    (void)o.transportId();
    (void)o.registrationNumber();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { Transport bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { AccessRole o("GET-AccessRole", "value", 2, 3.0);
    (void)o.grantPermission();
    (void)o.roleCode();
    (void)o.roleName();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { AccessRole bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { AuditLog o("GET-AuditLog", "value", 2, 3.0);
    (void)o.recordEvent();
    (void)o.eventId();
    (void)o.actorId();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { AuditLog bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { KPIReport o("GET-KPIReport", "value", 2, 3.0);
    (void)o.calculateOrderFulfillmentRate();
    (void)o.reportId();
    (void)o.period();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.averageOrdersPerPeriod();
    bool rejected = false;
    try { KPIReport bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { MaintenanceRequest o("GET-MaintenanceRequest", "value", 2, 3.0);
    (void)o.scheduleMaintenance();
    (void)o.requestId();
    (void)o.equipmentCode();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.warehouseCode();
    bool rejected = false;
    try { MaintenanceRequest bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { StockAlert o("GET-StockAlert", "value", 2, 3.0);
    (void)o.triggerIfLow();
    (void)o.alertId();
    (void)o.productSku();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { StockAlert bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { WarehouseSystem o("GET-WarehouseSystem", "value", 2, 3.0);
    (void)o.registerWarehouse();
    (void)o.systemId();
    (void)o.systemName();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { WarehouseSystem bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { Customer o("GET-Customer", "value", 2, 3.0);
    (void)o.validateContact();
    (void)o.customerId();
    (void)o.fullName();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { Customer bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { CustomerInvoice o("GET-CustomerInvoice", "value", 2, 3.0);
    (void)o.calculateDue();
    (void)o.invoiceId();
    (void)o.customerId();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.hasOutstandingBalance();
    (void)o.outstandingBalance();
    bool rejected = false;
    try { CustomerInvoice bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { PackingTask o("GET-PackingTask", "value", 2, 3.0);
    (void)o.calculatePackageCount();
    (void)o.taskId();
    (void)o.orderId();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { PackingTask bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { PickingTask o("GET-PickingTask", "value", 2, 3.0);
    (void)o.assignPicker();
    (void)o.taskId();
    (void)o.orderId();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.remainingQuantity();
    (void)o.pickedQuantity();
    bool rejected = false;
    try { PickingTask bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { ReturnRequest o("GET-ReturnRequest", "value", 2, 3.0);
    (void)o.approveReturn();
    (void)o.returnId();
    (void)o.orderId();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { ReturnRequest bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { Shipment o("GET-Shipment", "value", 2, 3.0);
    (void)o.dispatch();
    (void)o.shipmentId();
    (void)o.orderId();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { Shipment bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { Barcode o("GET-Barcode", "value", 2, 3.0);
    (void)o.validateBarcode();
    (void)o.barcodeValue();
    (void)o.format();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.isNumeric();
    bool rejected = false;
    try { Barcode bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { Brand o("GET-Brand", "value", 2, 3.0);
    (void)o.renameBrand();
    (void)o.brandCode();
    (void)o.name();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { Brand bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { ProductAttribute o("GET-ProductAttribute", "value", 2, 3.0);
    (void)o.validateValue();
    (void)o.attributeCode();
    (void)o.value();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.hasValue();
    bool rejected = false;
    try { ProductAttribute bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { ProductBundle o("GET-ProductBundle", "value", 2, 3.0);
    (void)o.calculateBundlePrice();
    (void)o.bundleCode();
    (void)o.name();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { ProductBundle bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { ProductCategory o("GET-ProductCategory", "value", 2, 3.0);
    (void)o.addProduct();
    (void)o.categoryCode();
    (void)o.name();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.isPopulated();
    bool rejected = false;
    try { ProductCategory bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { SerialNumber o("GET-SerialNumber", "value", 2, 3.0);
    (void)o.assignProduct();
    (void)o.serialCode();
    (void)o.productSku();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.isRegistered();
    bool rejected = false;
    try { SerialNumber bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { UnitOfMeasure o("GET-UnitOfMeasure", "value", 2, 3.0);
    (void)o.convertToBase();
    (void)o.unitCode();
    (void)o.symbol();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { UnitOfMeasure bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { ColdStorage o("GET-ColdStorage", "value", 2, 3.0);
    (void)o.validateTemperature();
    (void)o.storageCode();
    (void)o.temperature();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.isBelowFreezing();
    bool rejected = false;
    try { ColdStorage bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { LoadingDock o("GET-LoadingDock", "value", 2, 3.0);
    (void)o.openDock();
    (void)o.dockCode();
    (void)o.status();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.isOccupied();
    bool rejected = false;
    try { LoadingDock bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { Rack o("GET-Rack", "value", 2, 3.0);
    (void)o.calculateLoad();
    (void)o.rackCode();
    (void)o.zoneCode();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { Rack bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { Shelf o("GET-Shelf", "value", 2, 3.0);
    (void)o.calculateFreeVolume();
    (void)o.shelfCode();
    (void)o.rackCode();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    bool rejected = false;
    try { Shelf bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { StorageBin o("GET-StorageBin", "value", 2, 3.0);
    (void)o.calculateFreeCapacity();
    (void)o.binCode();
    (void)o.shelfCode();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.isEmpty();
    bool rejected = false;
    try { StorageBin bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
  { StorageCondition o("GET-StorageCondition", "value", 2, 3.0);
    (void)o.isSatisfiedBy();
    (void)o.conditionCode();
    (void)o.description();
    (void)o.quantity();
    (void)o.amount();
    (void)o.active();
    (void)o.relatedCount();
    (void)o.isTemperatureControlled();
    bool rejected = false;
    try { StorageCondition bad("", "value", 2, 3.0); } catch (const WarehouseException&) { rejected = true; }
    assert(rejected);
  }
}
