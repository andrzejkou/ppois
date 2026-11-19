#include "products/Product.h"
#include "storage/Warehouse.h"
#include "inventory/StockItem.h"
#include "orders/SalesOrder.h"
#include "exceptions/InsufficientStockException.h"

#include <iostream>

int main() {
    try {
        Product product("SKU-001", "Industrial filter", 120, 18.50);
        Warehouse warehouse("WH-01", "North distribution center", 500, 0.0);
        StockItem stock("ST-001", product.sku(), 120, product.amount());
        SalesOrder order("SO-1001", "CUSTOMER-7", 0, 0.0);

        stock.decreaseStock(5);
        std::cout << "Warehouse management system\n"
                  << "Product: " << product.name() << '\n'
                  << "Available stock: " << stock.quantity() << '\n'
                  << "Warehouse capacity available: "
                  << warehouse.calculateFreeCapacity() << '\n'
                  << "Order: " << order.orderId() << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
