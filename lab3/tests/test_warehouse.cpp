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
 std::cout<<"Warehouse domain tests passed\n";
}
