#pragma once

#include "Menu.h"

#include "inventory/StockItem.h"
#include "inventory/StockLot.h"
#include "orders/SalesOrder.h"
#include "orders/Customer.h"
#include "people/QualityInspector.h"
#include "people/Employee.h"
#include "people/WarehouseManager.h"
#include "products/Product.h"
#include "procurement/DeliveryNote.h"
#include "procurement/GoodsReceipt.h"
#include "procurement/PurchaseOrder.h"
#include "procurement/PurchaseRequest.h"
#include "procurement/Supplier.h"
#include "procurement/SupplierContract.h"
#include "storage/StorageZone.h"
#include "storage/Warehouse.h"

#include <string>
#include <vector>

class Application {
public:
    Application();
    void run();

private:
    struct ReceiptSummary {
        std::string receiptId;
        std::string purchaseOrderId;
        std::string inspectorId;
        int acceptedQuantity{0};
        int rejectedQuantity{0};
        bool completed{false};
        std::string rejectionReason;
    };

    struct ProductRecord {
        Product product;
        StockItem stock;

        ProductRecord(const std::string& sku,
                      const std::string& name,
                      double unitPrice,
                      int reorderLevel,
                      const std::string& warehouseCode,
                      const std::string& binCode,
                      int initialStock = 0,
                      const std::string& categoryCode = {},
                      const std::string& brandCode = {});
    };

    Menu menu_;
    Warehouse warehouse_;
    std::vector<ProductRecord> products_;
    std::vector<SalesOrder> orders_;
    std::vector<Employee> employees_;
    std::vector<Customer> customers_;

    // Procurement subsystem.
    std::vector<Supplier> suppliers_;
    std::vector<SupplierContract> supplierContracts_;
    std::vector<PurchaseRequest> purchaseRequests_;
    std::vector<PurchaseOrder> purchaseOrders_;
    std::vector<DeliveryNote> deliveryNotes_;
    std::vector<GoodsReceipt> goodsReceipts_;
    std::vector<ReceiptSummary> receiptHistory_;
    std::vector<StockLot> stockLots_;
    WarehouseManager warehouseManager_;
    QualityInspector qualityInspector_;

    void initializeDemoData();
    void loadData();
    void saveData() const;
    void handleChoice(int choice);

    void productMenu();
    void supplierMenu();
    void employeeMenu();
    void customerMenu();
    void procurementMenu();

    void listEmployees() const;
    void listCustomers() const;
    void addSupplier();
    void updateSupplier();
    void deleteSupplier();
    void addEmployee();
    void updateEmployee();
    void deleteEmployee();
    void addCustomer();
    void updateCustomer();
    void deleteCustomer();
    void updateProduct();
    void deleteProduct();

    void showProducts() const;
    void addProduct();
    void receiveProduct();
    void dispatchProduct();
    void createOrder();
    void showOrders() const;
    void shipOrder();
    void showWarehouseInfo() const;

    void showSuppliers() const;
    void createPurchaseRequest();
    void approvePurchaseRequest();
    void createPurchaseOrder();
    void receivePurchaseOrder();
    void showProcurement() const;

    ProductRecord* findProduct(const std::string& sku);
    const ProductRecord* findProduct(const std::string& sku) const;
    Supplier* findSupplier(const std::string& supplierId);
    Employee* findEmployee(const std::string& employeeId);
    Customer* findCustomer(const std::string& customerId);
    PurchaseRequest* findPurchaseRequest(const std::string& requestId);
    PurchaseOrder* findPurchaseOrder(const std::string& orderId);

    static std::string readString(const std::string& prompt);
    static int readInt(const std::string& prompt);
    static double readNonNegativeDouble(const std::string& prompt);
    static const char* orderStatusName(SalesOrderStatus status);
    static const char* purchaseRequestStatusName(PurchaseRequestStatus status);
    static const char* purchaseOrderStatusName(PurchaseOrderStatus status);
};
