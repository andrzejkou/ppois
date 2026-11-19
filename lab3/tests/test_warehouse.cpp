#include "products/Product.h"
#include "inventory/StockItem.h"
#include "storage/Warehouse.h"
#include "exceptions/WarehouseException.h"

#include <cassert>
#include <iostream>

int main() {
    Product product("SKU-1", "Bolt", 25, 2.5);
    assert(product.changePrice());
    assert(product.quantity() == 25);
    assert(product.isAvailableForSale(3));
    assert(product.quantity() == 28);
    product.addRelatedId("CAT-1");
    assert(product.hasRelatedId("CAT-1"));
    assert(product.relatedCount() == 1);

    StockItem stock("ST-1", "SKU-1", 10, 2.5);
    assert(stock.increaseStock());
    assert(stock.quantity() == 10);
    assert(stock.decreaseStock(3));
    assert(stock.quantity() == 7);

    Warehouse warehouse("WH-1", "Main warehouse", 100, 0.0);
    assert(warehouse.calculateFreeCapacity() == 0.0);
    assert(warehouse.canAccept(5));

    bool rejected = false;
    try { Product invalid("", "Broken", 0, 0.0); }
    catch (const WarehouseException&) { rejected = true; }
    assert(rejected);

    std::cout << "All warehouse smoke tests passed.\n";
    return 0;
}
