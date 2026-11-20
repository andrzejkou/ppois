#include "products/Product.h"
#include "storage/Warehouse.h"
#include "storage/StorageZone.h"
#include "inventory/StockItem.h"
#include "orders/SalesOrder.h"
#include <iostream>
int main(){try{Product p("SKU-100","Industrial filter",18.50,10);Warehouse w("WH-01","North distribution center",1000);StorageZone zone("Z-A","Ambient",800,20);w.addZone(zone);w.receive(120);StockItem stock("ST-100",p.sku(),w.code(),"A-01-01",120);SalesOrder order("SO-1001","CUSTOMER-7");order.addLine(SalesOrderLine(p.sku(),5,p.unitPrice()));order.confirm();std::cout<<"Warehouse Management System\nProduct: "<<p.name()<<"\nAvailable stock: "<<stock.available()<<"\nOrder total: "<<order.calculateTotal()<<"\nWarehouse occupancy: "<<w.occupancyRate()*100<<"%\n";}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}return 0;}
