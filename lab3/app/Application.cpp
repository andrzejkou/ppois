#include "Application.h"
#include "DomainModules.h"

#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
std::string currentDate() {
  const std::time_t now = std::time(nullptr);
  std::tm localTime{};
#if defined(_WIN32)
  localtime_s(&localTime, &now);
#else
  localtime_r(&now, &localTime);
#endif
  std::ostringstream result;
  result << std::put_time(&localTime, "%Y-%m-%d");
  return result.str();
}

std::time_t parseDate(const std::string &date) {
  if (date.size() != 10) {
    return static_cast<std::time_t>(-1);
  }

  std::tm parsed{};
  std::istringstream input(date);
  input >> std::get_time(&parsed, "%Y-%m-%d");
  if (input.fail()) {
    return static_cast<std::time_t>(-1);
  }

  parsed.tm_isdst = -1;
  const std::time_t result = std::mktime(&parsed);
  if (result <= 0) {
    return static_cast<std::time_t>(-1);
  }

  std::tm normalized{};
#if defined(_WIN32)
  localtime_s(&normalized, &result);
#else
  localtime_r(&result, &normalized);
#endif
  std::ostringstream formatted;
  formatted << std::put_time(&normalized, "%Y-%m-%d");
  return formatted.str() == date ? result : static_cast<std::time_t>(-1);
}

std::filesystem::path dataDirectory() {
  return std::filesystem::current_path() / "data";
}

std::string csvEscape(const std::string &value) {
  if (value.find_first_of(",\"\r\n") == std::string::npos)
    return value;
  std::string escaped = "\"";
  for (char ch : value) {
    if (ch == '\"')
      escaped += "\"\"";
    else
      escaped += ch;
  }
  escaped += '\"';
  return escaped;
}

std::vector<std::string> parseCsvRow(const std::string &line) {
  std::vector<std::string> fields;
  std::string field;
  bool quoted = false;
  for (std::size_t i = 0; i < line.size(); ++i) {
    const char ch = line[i];
    if (quoted) {
      if (ch == '\"' && i + 1 < line.size() && line[i + 1] == '\"') {
        field += '\"';
        ++i;
      } else if (ch == '\"')
        quoted = false;
      else
        field += ch;
    } else if (ch == '\"')
      quoted = true;
    else if (ch == ',') {
      fields.push_back(field);
      field.clear();
    } else
      field += ch;
  }
  fields.push_back(field);
  return fields;
}

std::vector<std::vector<std::string>>
readCsv(const std::filesystem::path &path) {
  std::vector<std::vector<std::string>> rows;
  std::ifstream input(path);
  if (!input)
    return rows;
  std::string line;
  bool header = true;
  while (std::getline(input, line)) {
    if (header) {
      header = false;
      continue;
    }
    if (line.empty())
      continue;
    rows.push_back(parseCsvRow(line));
  }
  return rows;
}

void writeCsv(const std::filesystem::path &path,
              const std::vector<std::string> &header,
              const std::vector<std::vector<std::string>> &rows) {
  std::filesystem::create_directories(path.parent_path());
  const auto temporary = path.string() + ".tmp";
  std::ofstream output(temporary, std::ios::trunc);
  if (!output)
    throw std::runtime_error("Cannot open data file for writing: " +
                             path.string());
  auto writeRow = [&output](const std::vector<std::string> &row) {
    for (std::size_t i = 0; i < row.size(); ++i) {
      if (i)
        output << ',';
      output << csvEscape(row[i]);
    }
    output << '\n';
  };
  writeRow(header);
  for (const auto &row : rows)
    writeRow(row);
  output.close();
  if (!output)
    throw std::runtime_error("Failed while writing data file: " +
                             path.string());
  std::error_code error;
  std::filesystem::rename(temporary, path, error);
  if (error) {
    std::filesystem::remove(path, error);
    error.clear();
    std::filesystem::rename(temporary, path, error);
    if (error)
      throw std::runtime_error("Cannot replace data file: " + path.string());
  }
}

std::filesystem::path inputDataFile(const std::string &filename) {
  const auto live = dataDirectory() / filename;
  if (std::filesystem::exists(live))
    return live;
  return dataDirectory() / "demo" / filename;
}

std::string field(const std::vector<std::string> &row, std::size_t index,
                  const std::string &fallback = {}) {
  return index < row.size() ? row[index] : fallback;
}

int toInt(const std::string &value, int fallback = 0) {
  try {
    return std::stoi(value);
  } catch (...) {
    return fallback;
  }
}

double toDouble(const std::string &value, double fallback = 0.0) {
  try {
    return std::stod(value);
  } catch (...) {
    return fallback;
  }
}

std::vector<std::string> splitSemicolon(const std::string &value) {
  std::vector<std::string> result;
  std::istringstream input(value);
  std::string item;
  while (std::getline(input, item, ';'))
    if (!item.empty())
      result.push_back(item);
  return result;
}

std::string joinSemicolon(const std::vector<std::string> &values) {
  std::ostringstream output;
  for (std::size_t i = 0; i < values.size(); ++i) {
    if (i)
      output << ';';
    output << values[i];
  }
  return output.str();
}
} // namespace

Application::ProductRecord::ProductRecord(
    const std::string &sku, const std::string &name, double unitPrice,
    int reorderLevel, const std::string &warehouseCode,
    const std::string &binCode, int initialStock,
    const std::string &categoryCode, const std::string &brandCode)
    : product(sku, name, unitPrice, reorderLevel, categoryCode, brandCode),
      stock("ST-" + sku, sku, warehouseCode, binCode, initialStock) {}

Application::Application()
    : warehouse_("WH-01", "North distribution center", 1000.0),
      warehouseManager_("EMP-MGR-01", "OPS", 100000.0, 4),
      qualityInspector_("EMP-QA-01", "QA-CERT-01", "2099-12-31") {
  initializeDemoData();
}

void Application::initializeDemoData() {
  StorageZone ambientZone("Z-A", "Ambient", 800.0);
  warehouse_.addZone(ambientZone);
  loadData();

  if (products_.empty()) {
    products_.emplace_back("SKU-100", "Industrial filter", 18.50, 10,
                           warehouse_.code(), "A-01-01", 120);
  }
  if (suppliers_.empty()) {
    suppliers_.emplace_back("SUP-001", "Industrial Supplies Ltd",
                            "orders@industrial.example", 7, 4.7);
  }
  if (employees_.empty()) {
    employees_.emplace_back("EMP-001", "Alex Morgan", "Storekeeper", 18.50);
    employees_.emplace_back("EMP-002", "Taylor Reed", "Warehouse worker",
                            16.00);
  }
  if (customers_.empty()) {
    customers_.emplace_back("CUST-001", "Northwind Retail", 0, 0.0);
  }

  if (supplierContracts_.empty()) {
    const auto contractRows = readCsv(inputDataFile("supplier_contracts.csv"));
    for (const auto &row : contractRows) {
      if (row.size() < 6)
        continue;
      supplierContracts_.emplace_back(
          field(row, 0), field(row, 1), field(row, 2), field(row, 3),
          toDouble(field(row, 4)), toDouble(field(row, 5)));
    }
    for (const auto &supplier : suppliers_) {
      const bool hasContract =
          std::any_of(supplierContracts_.begin(), supplierContracts_.end(),
                      [&supplier](const SupplierContract &contract) {
                        return contract.supplierId() == supplier.supplierId();
                      });
      if (!hasContract) {
        supplierContracts_.emplace_back("CON-" + supplier.supplierId(),
                                        supplier.supplierId(), "2020-01-01",
                                        "2099-12-31", 0.0, 0.01);
      }
    }
  }

  int occupied = 0;
  for (const auto &record : products_)
    occupied += record.stock.onHand();
  // Quarantined lots occupy physical warehouse space but are not saleable
  // stock.
  for (const auto &lot : stockLots_) {
    if (lot.quarantined())
      occupied += lot.quantity();
  }
  if (occupied > 0)
    warehouse_.receive(occupied);

  // A first run creates writable state files from the demonstration templates.
  saveData();
}

void Application::loadData() {
  products_.clear();
  suppliers_.clear();
  employees_.clear();
  customers_.clear();
  orders_.clear();
  purchaseRequests_.clear();
  purchaseOrders_.clear();
  supplierContracts_.clear();
  stockLots_.clear();
  deliveryNotes_.clear();
  receiptHistory_.clear();

  for (const auto &row : readCsv(inputDataFile("products.csv"))) {
    if (row.size() < 8)
      continue;
    const std::string sku = field(row, 0);
    ProductRecord record(sku, field(row, 1), toDouble(field(row, 2)),
                         toInt(field(row, 3)), field(row, 6, warehouse_.code()),
                         field(row, 7, "UNASSIGNED"), toInt(field(row, 8)),
                         field(row, 4), field(row, 5));
    const int reserved = toInt(field(row, 9));
    if (reserved > 0)
      record.stock.reserve(reserved);
    if (field(row, 10, "1") == "0")
      record.product.deactivate();
    for (const auto &barcode : splitSemicolon(field(row, 11))) {
      record.product.addBarcode(barcode);
    }
    products_.push_back(std::move(record));
  }

  for (const auto &row : readCsv(inputDataFile("suppliers.csv"))) {
    if (row.size() < 5)
      continue;
    Supplier supplier(field(row, 0), field(row, 1), field(row, 2),
                      toInt(field(row, 3)), toDouble(field(row, 4)));
    if (field(row, 5, "1") == "0")
      supplier.deactivate();
    suppliers_.push_back(std::move(supplier));
  }

  for (const auto &row : readCsv(inputDataFile("employees.csv"))) {
    if (row.size() < 4)
      continue;
    Employee employee(field(row, 0), field(row, 1), field(row, 2),
                      toDouble(field(row, 3)));
    for (const auto &skill : splitSemicolon(field(row, 5)))
      employee.addSkill(skill);
    if (field(row, 4, "1") == "0")
      employee.deactivate();
    employees_.push_back(std::move(employee));
  }

  for (const auto &row : readCsv(inputDataFile("customers.csv"))) {
    if (row.size() < 4)
      continue;
    customers_.emplace_back(field(row, 0), field(row, 1), toInt(field(row, 2)),
                            toDouble(field(row, 3)));
  }

  // Restore sales orders after loading the product/customer catalogs.
  const auto orderRows = readCsv(inputDataFile("sales_orders.csv"));
  const auto orderLineRows = readCsv(inputDataFile("sales_order_lines.csv"));
  for (const auto &row : orderRows) {
    if (row.size() < 3)
      continue;
    SalesOrder order(field(row, 0), field(row, 1));
    for (const auto &line : orderLineRows) {
      if (field(line, 0) != order.orderId() || line.size() < 5)
        continue;
      SalesOrderLine item(field(line, 1), toInt(field(line, 2)),
                          toDouble(field(line, 3)));
      item.applyDiscount(toDouble(field(line, 4)));
      order.addLine(item);
    }
    const std::string status = field(row, 2, "Draft");
    if (status == "Confirmed" || status == "Picking" || status == "Shipped")
      order.confirm();
    if (status == "Picking" || status == "Shipped")
      order.beginPicking();
    if (status == "Shipped")
      order.markShipped();
    if (status == "Cancelled")
      order.cancel();
    orders_.push_back(std::move(order));
  }

  const auto requestRows = readCsv(inputDataFile("purchase_requests.csv"));
  const auto requestLineRows =
      readCsv(inputDataFile("purchase_request_lines.csv"));
  for (const auto &row : requestRows) {
    if (row.size() < 6)
      continue;
    PurchaseRequest request(field(row, 0), field(row, 1), field(row, 2));
    for (const auto &line : requestLineRows) {
      if (field(line, 0) == request.requestId() && line.size() >= 4) {
        request.addLine(
            {field(line, 1), toInt(field(line, 2)), toDouble(field(line, 3))});
      }
    }
    const std::string status = field(row, 3, "Draft");
    if (status != "Draft")
      request.submit();
    if (status == "Approved")
      request.approve(field(row, 4));
    else if (status == "Rejected")
      request.reject(field(row, 4), field(row, 5));
    else if (status == "Cancelled")
      request.cancel();
    purchaseRequests_.push_back(std::move(request));
  }

  const auto poRows = readCsv(inputDataFile("purchase_orders.csv"));
  const auto poLineRows = readCsv(inputDataFile("purchase_order_lines.csv"));
  for (const auto &row : poRows) {
    if (row.size() < 3)
      continue;
    PurchaseOrder order(field(row, 0), field(row, 1));
    for (const auto &line : poLineRows) {
      if (field(line, 0) == order.id() && line.size() >= 5) {
        order.addLine(PurchaseOrderLine(field(line, 1), toInt(field(line, 2)),
                                        toDouble(field(line, 4))));
      }
    }
    const std::string status = field(row, 2, "Draft");
    if (status != "Draft")
      order.approve();
    for (const auto &line : poLineRows) {
      if (field(line, 0) == order.id() && line.size() >= 5) {
        const int received = toInt(field(line, 3));
        if (received > 0)
          order.registerReceipt(field(line, 1), received);
      }
    }
    if (status == "Cancelled" &&
        order.status() != PurchaseOrderStatus::Received)
      order.cancel();
    purchaseOrders_.push_back(std::move(order));
  }

  for (const auto &row : readCsv(inputDataFile("stock_lots.csv"))) {
    if (row.size() < 5)
      continue;
    StockLot lot(field(row, 0), field(row, 1), toInt(field(row, 2)),
                 static_cast<std::time_t>(std::stoll(field(row, 3))),
                 toDouble(field(row, 4)));
    if (field(row, 5, "0") == "1")
      lot.markQuarantined();
    stockLots_.push_back(std::move(lot));
  }

  for (const auto &row : readCsv(inputDataFile("delivery_notes.csv"))) {
    if (row.size() < 5)
      continue;
    DeliveryNote note(field(row, 0), field(row, 1), field(row, 2));
    const auto lineRows = readCsv(inputDataFile("delivery_note_lines.csv"));
    for (const auto &line : lineRows) {
      if (field(line, 0) == note.noteNumber() && line.size() >= 3)
        note.addLine({field(line, 1), toInt(field(line, 2))});
    }
    if (field(row, 4) == "1")
      note.markReceived(field(row, 3));
    deliveryNotes_.push_back(std::move(note));
  }

  for (const auto &row : readCsv(inputDataFile("receipt_history.csv"))) {
    if (row.size() < 7)
      continue;
    receiptHistory_.push_back({field(row, 0), field(row, 1), field(row, 2),
                               toInt(field(row, 3)), toInt(field(row, 4)),
                               field(row, 5) == "1", field(row, 6)});
  }
}

void Application::saveData() const {
  std::vector<std::vector<std::string>> rows;
  for (const auto &record : products_) {
    rows.push_back({record.product.sku(), record.product.name(),
                    std::to_string(record.product.unitPrice()),
                    std::to_string(record.product.reorderLevel()),
                    record.product.categoryCode(), record.product.brandCode(),
                    record.stock.warehouseCode(), record.stock.binCode(),
                    std::to_string(record.stock.onHand()),
                    std::to_string(record.stock.reserved()),
                    record.product.active() ? "1" : "0",
                    joinSemicolon(record.product.barcodes())});
  }
  writeCsv(dataDirectory() / "products.csv",
           {"sku", "name", "unit_price", "reorder_level", "category_code",
            "brand_code", "warehouse_code", "bin_code", "on_hand", "reserved",
            "active", "barcodes"},
           rows);

  rows.clear();
  for (const auto &supplier : suppliers_)
    rows.push_back({supplier.supplierId(), supplier.companyName(),
                    supplier.contactEmail(),
                    std::to_string(supplier.leadTimeDays()),
                    std::to_string(supplier.reliabilityScore()),
                    supplier.active() ? "1" : "0"});
  writeCsv(dataDirectory() / "suppliers.csv",
           {"supplier_id", "company_name", "contact_email", "lead_time_days",
            "reliability", "active"},
           rows);

  rows.clear();
  for (const auto &employee : employees_)
    rows.push_back({employee.employeeId(), employee.fullName(),
                    employee.position(), std::to_string(employee.hourlyRate()),
                    employee.active() ? "1" : "0",
                    joinSemicolon(employee.skills())});
  writeCsv(dataDirectory() / "employees.csv",
           {"employee_id", "full_name", "position", "hourly_rate", "active",
            "skills"},
           rows);

  rows.clear();
  for (const auto &customer : customers_)
    rows.push_back({customer.customerId(), customer.fullName(),
                    std::to_string(customer.quantity()),
                    std::to_string(customer.amount())});
  writeCsv(dataDirectory() / "customers.csv",
           {"customer_id", "full_name", "loyalty_quantity", "amount"}, rows);

  rows.clear();
  std::vector<std::vector<std::string>> lineRows;
  for (const auto &order : orders_) {
    rows.push_back(
        {order.orderId(), order.customerId(), orderStatusName(order.status())});
    for (const auto &line : order.lines())
      lineRows.push_back(
          {order.orderId(), line.sku(), std::to_string(line.quantity()),
           std::to_string(line.unitPrice()), std::to_string(line.discount())});
  }
  writeCsv(dataDirectory() / "sales_orders.csv",
           {"order_id", "customer_id", "status"}, rows);
  writeCsv(dataDirectory() / "sales_order_lines.csv",
           {"order_id", "sku", "quantity", "unit_price", "discount_percent"},
           lineRows);

  rows.clear();
  lineRows.clear();
  for (const auto &request : purchaseRequests_) {
    rows.push_back({request.requestId(), request.departmentCode(),
                    request.requesterId(),
                    purchaseRequestStatusName(request.status()),
                    request.approverId(), request.rejectionReason()});
    for (const auto &line : request.lines())
      lineRows.push_back({request.requestId(), line.sku,
                          std::to_string(line.quantity),
                          std::to_string(line.estimatedUnitPrice)});
  }
  writeCsv(dataDirectory() / "purchase_requests.csv",
           {"request_id", "department", "requester_id", "status", "approver_id",
            "rejection_reason"},
           rows);
  writeCsv(dataDirectory() / "purchase_request_lines.csv",
           {"request_id", "sku", "quantity", "estimated_unit_price"}, lineRows);

  rows.clear();
  lineRows.clear();
  for (const auto &order : purchaseOrders_) {
    rows.push_back({order.id(), order.supplierId(),
                    purchaseOrderStatusName(order.status())});
    for (const auto &line : order.lines())
      lineRows.push_back(
          {order.id(), line.sku(), std::to_string(line.ordered()),
           std::to_string(line.received()),
           std::to_string(line.ordered() > 0 ? line.lineTotal() / line.ordered()
                                             : 0.0)});
  }
  writeCsv(dataDirectory() / "purchase_orders.csv",
           {"order_id", "supplier_id", "status"}, rows);
  writeCsv(dataDirectory() / "purchase_order_lines.csv",
           {"order_id", "sku", "ordered", "received", "unit_cost"}, lineRows);

  rows.clear();
  for (const auto &lot : stockLots_)
    rows.push_back({lot.lotNumber(), lot.sku(), std::to_string(lot.quantity()),
                    std::to_string(static_cast<long long>(lot.expiryDate())),
                    std::to_string(lot.unitCost()),
                    lot.quarantined() ? "1" : "0"});
  writeCsv(dataDirectory() / "stock_lots.csv",
           {"lot_number", "sku", "quantity", "expiry_epoch", "unit_cost",
            "quarantined"},
           rows);

  rows.clear();
  lineRows.clear();
  for (const auto &note : deliveryNotes_) {
    rows.push_back({note.noteNumber(), note.supplierId(),
                    note.purchaseOrderId(), note.receivedDate(),
                    note.received() ? "1" : "0"});
    for (const auto &line : note.lines())
      lineRows.push_back({note.noteNumber(), line.sku,
                          std::to_string(line.deliveredQuantity)});
  }
  writeCsv(dataDirectory() / "delivery_notes.csv",
           {"note_number", "supplier_id", "purchase_order_id", "received_date",
            "received"},
           rows);
  writeCsv(dataDirectory() / "delivery_note_lines.csv",
           {"note_number", "sku", "quantity"}, lineRows);

  rows.clear();
  for (const auto &receipt : receiptHistory_)
    rows.push_back({receipt.receiptId, receipt.purchaseOrderId,
                    receipt.inspectorId,
                    std::to_string(receipt.acceptedQuantity),
                    std::to_string(receipt.rejectedQuantity),
                    receipt.completed ? "1" : "0", receipt.rejectionReason});
  // Include current-session receipts which have not yet been copied to history.
  for (const auto &receipt : goodsReceipts_) {
    const bool alreadySaved =
        std::any_of(receiptHistory_.begin(), receiptHistory_.end(),
                    [&receipt](const ReceiptSummary &item) {
                      return item.receiptId == receipt.receiptId();
                    });
    if (!alreadySaved)
      rows.push_back(
          {receipt.receiptId(), receipt.purchaseOrderId(),
           receipt.inspectorId(), std::to_string(receipt.acceptedQuantity()),
           std::to_string(receipt.rejectedQuantity()),
           receipt.completed() ? "1" : "0", receipt.rejectionReason()});
  }
  writeCsv(dataDirectory() / "receipt_history.csv",
           {"receipt_id", "purchase_order_id", "inspector_id",
            "accepted_quantity", "rejected_quantity", "completed",
            "rejection_reason"},
           rows);

  rows.clear();
  for (const auto &contract : supplierContracts_)
    rows.push_back({contract.contractId(), contract.supplierId(),
                    contract.validFrom(), contract.validUntil(),
                    std::to_string(contract.minimumOrderValue()),
                    std::to_string(contract.latePenaltyRate())});
  writeCsv(dataDirectory() / "supplier_contracts.csv",
           {"contract_id", "supplier_id", "valid_from", "valid_until",
            "minimum_order_value", "late_penalty_rate"},
           rows);
}

void Application::run() {
  std::cout << "Warehouse Management System started.\n";
  std::cout << "Data directory: " << dataDirectory().string() << "\n";
  std::cout << "Demo records are loaded from data/demo on the first run.\n";

  while (true) {
    menu_.printMainMenu();
    const int choice = menu_.readChoice();
    if (choice == 0) {
      saveData();
      std::cout << "Data saved. Application closed.\n";
      return;
    }

    try {
      handleChoice(choice);
      saveData();
    } catch (const std::exception &error) {
      std::cout << "Operation failed: " << error.what() << '\n';
    }
  }
}

void Application::handleChoice(int choice) {
  switch (choice) {
  case 1:
    productMenu();
    break;
  case 2:
    supplierMenu();
    break;
  case 3:
    employeeMenu();
    break;
  case 4:
    customerMenu();
    break;
  case 5: {
    bool back = false;
    while (!back) {
      menu_.printStockMenu();
      switch (menu_.readChoice()) {
      case 1:
        showProducts();
        break;
      case 2:
        receiveProduct();
        break;
      case 3:
        dispatchProduct();
        break;
      case 0:
        back = true;
        break;
      default:
        std::cout << "Unknown stock option.\n";
        break;
      }
      saveData();
    }
    break;
  }
  case 6: {
    bool back = false;
    while (!back) {
      menu_.printSalesMenu();
      switch (menu_.readChoice()) {
      case 1:
        createOrder();
        break;
      case 2:
        showOrders();
        break;
      case 3:
        shipOrder();
        break;
      case 0:
        back = true;
        break;
      default:
        std::cout << "Unknown sales option.\n";
        break;
      }
      saveData();
    }
    break;
  }
  case 7:
    procurementMenu();
    break;
  case 8:
    showWarehouseInfo();
    break;
  case 9:
    runExtendedDomainDemonstration();
    break;
  case 10:
    saveData();
    std::cout << "All application data saved.\n";
    break;
  default:
    std::cout << "Unknown menu option.\n";
    break;
  }
}

void Application::productMenu() {
  bool back = false;
  while (!back) {
    menu_.printCrudMenu("PRODUCT CATALOG");
    switch (menu_.readChoice()) {
    case 1:
      addProduct();
      break;
    case 2:
      showProducts();
      break;
    case 3: {
      const std::string sku = readString("SKU to find: ");
      const ProductRecord *record = findProduct(sku);
      if (!record)
        std::cout << "Product not found.\n";
      else
        std::cout << record->product.sku() << " | " << record->product.name()
                  << " | price=" << record->product.unitPrice()
                  << " | on hand=" << record->stock.onHand()
                  << " | reserved=" << record->stock.reserved()
                  << " | active=" << (record->product.active() ? "yes" : "no")
                  << '\n';
      break;
    }
    case 4:
      updateProduct();
      break;
    case 5:
      deleteProduct();
      break;
    case 0:
      back = true;
      break;
    default:
      std::cout << "Unknown product option.\n";
      break;
    }
    saveData();
  }
}

void Application::supplierMenu() {
  bool back = false;
  while (!back) {
    menu_.printCrudMenu("SUPPLIER DIRECTORY");
    switch (menu_.readChoice()) {
    case 1:
      addSupplier();
      break;
    case 2:
      showSuppliers();
      break;
    case 3: {
      const std::string id = readString("Supplier ID to find: ");
      Supplier *supplier = findSupplier(id);
      if (!supplier)
        std::cout << "Supplier not found.\n";
      else
        std::cout << supplier->supplierId() << " | " << supplier->companyName()
                  << " | " << supplier->contactEmail()
                  << " | lead time=" << supplier->leadTimeDays()
                  << " days | rating=" << supplier->reliabilityScore()
                  << " | active=" << (supplier->active() ? "yes" : "no")
                  << '\n';
      break;
    }
    case 4:
      updateSupplier();
      break;
    case 5:
      deleteSupplier();
      break;
    case 0:
      back = true;
      break;
    default:
      std::cout << "Unknown supplier option.\n";
      break;
    }
    saveData();
  }
}

void Application::employeeMenu() {
  bool back = false;
  while (!back) {
    menu_.printCrudMenu("EMPLOYEE DIRECTORY");
    switch (menu_.readChoice()) {
    case 1:
      addEmployee();
      break;
    case 2:
      listEmployees();
      break;
    case 3: {
      const std::string id = readString("Employee ID to find: ");
      Employee *employee = findEmployee(id);
      if (!employee)
        std::cout << "Employee not found.\n";
      else
        std::cout << employee->employeeId() << " | " << employee->fullName()
                  << " | " << employee->position()
                  << " | rate=" << employee->hourlyRate()
                  << " | active=" << (employee->active() ? "yes" : "no")
                  << '\n';
      break;
    }
    case 4:
      updateEmployee();
      break;
    case 5:
      deleteEmployee();
      break;
    case 0:
      back = true;
      break;
    default:
      std::cout << "Unknown employee option.\n";
      break;
    }
    saveData();
  }
}

void Application::customerMenu() {
  bool back = false;
  while (!back) {
    menu_.printCrudMenu("CUSTOMER DIRECTORY");
    switch (menu_.readChoice()) {
    case 1:
      addCustomer();
      break;
    case 2:
      listCustomers();
      break;
    case 3: {
      const std::string id = readString("Customer ID to find: ");
      Customer *customer = findCustomer(id);
      if (!customer)
        std::cout << "Customer not found.\n";
      else
        std::cout << customer->customerId() << " | " << customer->fullName()
                  << " | loyalty quantity=" << customer->quantity()
                  << " | amount=" << customer->amount() << '\n';
      break;
    }
    case 4:
      updateCustomer();
      break;
    case 5:
      deleteCustomer();
      break;
    case 0:
      back = true;
      break;
    default:
      std::cout << "Unknown customer option.\n";
      break;
    }
    saveData();
  }
}

void Application::procurementMenu() {
  bool back = false;
  while (!back) {
    menu_.printProcurementMenu();
    switch (menu_.readChoice()) {
    case 1:
      showSuppliers();
      break;
    case 2:
      createPurchaseRequest();
      break;
    case 3:
      approvePurchaseRequest();
      break;
    case 4:
      createPurchaseOrder();
      break;
    case 5:
      receivePurchaseOrder();
      break;
    case 6:
      showProcurement();
      break;
    case 0:
      back = true;
      break;
    default:
      std::cout << "Unknown procurement option.\n";
      break;
    }
    saveData();
  }
}

Application::ProductRecord *Application::findProduct(const std::string &sku) {
  const auto it = std::find_if(products_.begin(), products_.end(),
                               [&sku](const ProductRecord &record) {
                                 return record.product.sku() == sku;
                               });
  return it == products_.end() ? nullptr : &*it;
}

const Application::ProductRecord *
Application::findProduct(const std::string &sku) const {
  const auto it = std::find_if(products_.begin(), products_.end(),
                               [&sku](const ProductRecord &record) {
                                 return record.product.sku() == sku;
                               });
  return it == products_.end() ? nullptr : &*it;
}

Supplier *Application::findSupplier(const std::string &supplierId) {
  const auto it = std::find_if(suppliers_.begin(), suppliers_.end(),
                               [&supplierId](const Supplier &supplier) {
                                 return supplier.supplierId() == supplierId;
                               });
  return it == suppliers_.end() ? nullptr : &*it;
}

Employee *Application::findEmployee(const std::string &employeeId) {
  const auto it = std::find_if(employees_.begin(), employees_.end(),
                               [&employeeId](const Employee &employee) {
                                 return employee.employeeId() == employeeId;
                               });
  return it == employees_.end() ? nullptr : &*it;
}

Customer *Application::findCustomer(const std::string &customerId) {
  const auto it = std::find_if(customers_.begin(), customers_.end(),
                               [&customerId](const Customer &customer) {
                                 return customer.customerId() == customerId;
                               });
  return it == customers_.end() ? nullptr : &*it;
}

PurchaseRequest *
Application::findPurchaseRequest(const std::string &requestId) {
  const auto it =
      std::find_if(purchaseRequests_.begin(), purchaseRequests_.end(),
                   [&requestId](const PurchaseRequest &request) {
                     return request.requestId() == requestId;
                   });
  return it == purchaseRequests_.end() ? nullptr : &*it;
}

PurchaseOrder *Application::findPurchaseOrder(const std::string &orderId) {
  const auto it = std::find_if(
      purchaseOrders_.begin(), purchaseOrders_.end(),
      [&orderId](const PurchaseOrder &order) { return order.id() == orderId; });
  return it == purchaseOrders_.end() ? nullptr : &*it;
}

std::string Application::readString(const std::string &prompt) {
  std::string value;
  std::cout << prompt;
  std::getline(std::cin, value);
  return value;
}

int Application::readInt(const std::string &prompt) {
  int value{};
  while (true) {
    std::cout << prompt;
    if (std::cin >> value) {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return value;
    }
    std::cout << "Invalid input. Enter an integer.\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}

double Application::readNonNegativeDouble(const std::string &prompt) {
  double value{};
  while (true) {
    std::cout << prompt;
    if (std::cin >> value && value >= 0.0) {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return value;
    }
    std::cout << "Enter a non-negative number.\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}

void Application::showProducts() const {
  if (products_.empty()) {
    std::cout << "Product catalog is empty.\n";
    return;
  }

  std::cout << '\n'
            << std::left << std::setw(16) << "SKU" << std::setw(26) << "Name"
            << std::setw(12) << "Price" << std::setw(12) << "On hand"
            << std::setw(12) << "Reserved"
            << "Available\n";
  std::cout << std::string(90, '-') << '\n';

  for (const auto &record : products_) {
    std::cout << std::left << std::setw(16) << record.product.sku()
              << std::setw(26) << record.product.name() << std::setw(12)
              << record.product.unitPrice() << std::setw(12)
              << record.stock.onHand() << std::setw(12)
              << record.stock.reserved() << record.stock.available() << '\n';
  }
}

void Application::addProduct() {
  const std::string sku = readString("SKU: ");
  if (sku.empty() || findProduct(sku)) {
    std::cout << "SKU is empty or already exists.\n";
    return;
  }

  const std::string name = readString("Product name: ");
  if (name.empty()) {
    std::cout << "Product name cannot be empty.\n";
    return;
  }

  const double price = readNonNegativeDouble("Unit price: ");
  const int reorderLevel = readInt("Reorder level: ");
  if (reorderLevel < 0) {
    std::cout << "Reorder level cannot be negative.\n";
    return;
  }

  const std::string categoryCode = readString("Category code (optional): ");
  const std::string brandCode = readString("Brand code (optional): ");
  const std::string binCode = readString("Storage bin code: ");
  if (binCode.empty()) {
    std::cout << "Storage bin code cannot be empty.\n";
    return;
  }
  products_.emplace_back(sku, name, price, reorderLevel, warehouse_.code(),
                         binCode, 0, categoryCode, brandCode);
  std::cout << "Product added.\n";
}

void Application::updateProduct() {
  const std::string sku = readString("SKU to update: ");
  ProductRecord *record = findProduct(sku);
  if (!record) {
    std::cout << "Product not found.\n";
    return;
  }

  const std::string name = readString("New name: ");
  const double price = readNonNegativeDouble("New unit price: ");
  const int reorderLevel = readInt("New reorder level: ");
  const std::string binCode = readString("New storage bin: ");
  if (name.empty() || reorderLevel < 0 || binCode.empty()) {
    std::cout << "Invalid product data.\n";
    return;
  }

  Product updated(sku, name, price, reorderLevel,
                  record->product.categoryCode(), record->product.brandCode());
  for (const auto &barcode : record->product.barcodes())
    updated.addBarcode(barcode);
  if (!record->product.active())
    updated.deactivate();
  record->product = std::move(updated);
  record->stock.moveTo(binCode);
  std::cout << "Product updated.\n";
}

void Application::deleteProduct() {
  const std::string sku = readString("SKU to delete: ");
  auto it = std::find_if(
      products_.begin(), products_.end(),
      [&sku](const ProductRecord &item) { return item.product.sku() == sku; });
  if (it == products_.end()) {
    std::cout << "Product not found.\n";
    return;
  }
  if (it->stock.onHand() != 0 || it->stock.reserved() != 0) {
    std::cout << "Cannot delete a product with stock or reservations. Dispatch "
                 "or adjust it first.\n";
    return;
  }
  const bool usedBySales = std::any_of(
      orders_.begin(), orders_.end(), [&sku](const SalesOrder &order) {
        return std::any_of(
            order.lines().begin(), order.lines().end(),
            [&sku](const SalesOrderLine &line) { return line.sku() == sku; });
      });
  const bool usedByPurchase = std::any_of(
      purchaseOrders_.begin(), purchaseOrders_.end(),
      [&sku](const PurchaseOrder &order) {
        return std::any_of(order.lines().begin(), order.lines().end(),
                           [&sku](const PurchaseOrderLine &line) {
                             return line.sku() == sku;
                           });
      });
  const bool usedByRequest = std::any_of(
      purchaseRequests_.begin(), purchaseRequests_.end(),
      [&sku](const PurchaseRequest &request) { return request.hasLine(sku); });
  const bool hasLots =
      std::any_of(stockLots_.begin(), stockLots_.end(),
                  [&sku](const StockLot &lot) { return lot.sku() == sku; });
  if (usedBySales || usedByPurchase || usedByRequest || hasLots) {
    std::cout << "Cannot delete a product referenced by orders, requests or "
                 "stock lots.\n";
    return;
  }
  products_.erase(it);
  std::cout << "Product deleted.\n";
}

void Application::addSupplier() {
  const std::string id = readString("Supplier ID: ");
  if (id.empty() || findSupplier(id)) {
    std::cout << "Supplier ID is empty or already exists.\n";
    return;
  }
  const std::string name = readString("Company name: ");
  const std::string email = readString("Contact email: ");
  const int leadTime = readInt("Lead time (days): ");
  const double rating = readNonNegativeDouble("Reliability rating (0..5): ");
  if (name.empty() || email.empty() || leadTime < 0 || rating > 5.0) {
    std::cout << "Invalid supplier data.\n";
    return;
  }
  suppliers_.emplace_back(id, name, email, leadTime, rating);
  supplierContracts_.emplace_back("CON-" + id, id, currentDate(), "2099-12-31",
                                  0.0, 0.01);
  std::cout << "Supplier and a standard contract were created.\n";
}

void Application::updateSupplier() {
  const std::string id = readString("Supplier ID to update: ");
  Supplier *supplier = findSupplier(id);
  if (!supplier) {
    std::cout << "Supplier not found.\n";
    return;
  }
  const std::string name = readString("New company name: ");
  const std::string email = readString("New contact email: ");
  const int leadTime = readInt("New lead time (days): ");
  const double rating =
      readNonNegativeDouble("New reliability rating (0..5): ");
  if (name.empty() || email.empty() || leadTime < 0 || rating > 5.0) {
    std::cout << "Invalid supplier data.\n";
    return;
  }
  Supplier updated(id, name, email, leadTime, rating);
  if (!supplier->active())
    updated.deactivate();
  *supplier = std::move(updated);
  std::cout << "Supplier updated. Historical delivery counters are reset.\n";
}

void Application::deleteSupplier() {
  const std::string id = readString("Supplier ID to delete: ");
  auto it = std::find_if(
      suppliers_.begin(), suppliers_.end(),
      [&id](const Supplier &item) { return item.supplierId() == id; });
  if (it == suppliers_.end()) {
    std::cout << "Supplier not found.\n";
    return;
  }
  const bool referenced = std::any_of(
      purchaseOrders_.begin(), purchaseOrders_.end(),
      [&id](const PurchaseOrder &order) { return order.supplierId() == id; });
  if (referenced) {
    std::cout
        << "Supplier is referenced by purchase orders; deletion is blocked.\n";
    return;
  }
  supplierContracts_.erase(
      std::remove_if(supplierContracts_.begin(), supplierContracts_.end(),
                     [&id](const SupplierContract &contract) {
                       return contract.supplierId() == id;
                     }),
      supplierContracts_.end());
  suppliers_.erase(it);
  std::cout << "Supplier and its contracts deleted.\n";
}

void Application::listEmployees() const {
  std::cout << "\n========== EMPLOYEES ==========\n";
  if (employees_.empty()) {
    std::cout << "No employees.\n";
    return;
  }
  for (const auto &employee : employees_) {
    std::cout << employee.employeeId() << " | " << employee.fullName() << " | "
              << employee.position() << " | rate=" << employee.hourlyRate()
              << " | " << (employee.active() ? "active" : "inactive")
              << " | skills: " << joinSemicolon(employee.skills()) << '\n';
  }
}

void Application::addEmployee() {
  const std::string id = readString("Employee ID: ");
  if (id.empty() || findEmployee(id)) {
    std::cout << "Employee ID is empty or already exists.\n";
    return;
  }
  const std::string name = readString("Full name: ");
  const std::string position = readString("Position: ");
  const double rate = readNonNegativeDouble("Hourly rate: ");
  if (name.empty() || position.empty()) {
    std::cout << "Name and position are required.\n";
    return;
  }
  employees_.emplace_back(id, name, position, rate);
  while (true) {
    const std::string skill = readString("Skill (empty to finish): ");
    if (skill.empty())
      break;
    employees_.back().addSkill(skill);
  }
  std::cout << "Employee created.\n";
}

void Application::updateEmployee() {
  const std::string id = readString("Employee ID to update: ");
  Employee *employee = findEmployee(id);
  if (!employee) {
    std::cout << "Employee not found.\n";
    return;
  }
  if (!employee->active()) {
    std::cout << "Inactive employee cannot be updated.\n";
    return;
  }
  const std::string name = readString("New full name: ");
  const std::string position = readString("New position: ");
  const double rate = readNonNegativeDouble("New hourly rate: ");
  if (name.empty() || position.empty()) {
    std::cout << "Name and position are required.\n";
    return;
  }
  Employee updated(id, name, position, rate);
  for (const auto &skill : employee->skills())
    updated.addSkill(skill);
  *employee = std::move(updated);
  std::cout << "Employee updated.\n";
}

void Application::deleteEmployee() {
  const std::string id = readString("Employee ID to delete: ");
  auto it = std::find_if(
      employees_.begin(), employees_.end(),
      [&id](const Employee &employee) { return employee.employeeId() == id; });
  if (it == employees_.end()) {
    std::cout << "Employee not found.\n";
    return;
  }
  const bool referenced = std::any_of(
      purchaseRequests_.begin(), purchaseRequests_.end(),
      [&id](const PurchaseRequest &request) {
        return request.requesterId() == id || request.approverId() == id;
      });
  if (referenced || id == warehouseManager_.employeeId() ||
      id == qualityInspector_.employeeId()) {
    std::cout << "Employee is referenced by an operation or system role; "
                 "deletion is blocked.\n";
    return;
  }
  employees_.erase(it);
  std::cout << "Employee deleted.\n";
}

void Application::listCustomers() const {
  std::cout << "\n========== CUSTOMERS ==========\n";
  if (customers_.empty()) {
    std::cout << "No customers.\n";
    return;
  }
  for (const auto &customer : customers_) {
    std::cout << customer.customerId() << " | " << customer.fullName()
              << " | loyalty=" << customer.quantity()
              << " | amount=" << customer.amount() << '\n';
  }
}

void Application::addCustomer() {
  const std::string id = readString("Customer ID: ");
  if (id.empty() || findCustomer(id)) {
    std::cout << "Customer ID is empty or already exists.\n";
    return;
  }
  const std::string name = readString("Full name / company: ");
  if (name.empty()) {
    std::cout << "Customer name is required.\n";
    return;
  }
  customers_.emplace_back(id, name);
  std::cout << "Customer created.\n";
}

void Application::updateCustomer() {
  const std::string id = readString("Customer ID to update: ");
  Customer *customer = findCustomer(id);
  if (!customer) {
    std::cout << "Customer not found.\n";
    return;
  }
  const std::string name = readString("New full name / company: ");
  if (name.empty()) {
    std::cout << "Customer name is required.\n";
    return;
  }
  *customer = Customer(id, name, customer->quantity(), customer->amount());
  std::cout << "Customer updated.\n";
}

void Application::deleteCustomer() {
  const std::string id = readString("Customer ID to delete: ");
  auto it = std::find_if(
      customers_.begin(), customers_.end(),
      [&id](const Customer &customer) { return customer.customerId() == id; });
  if (it == customers_.end()) {
    std::cout << "Customer not found.\n";
    return;
  }
  const bool referenced = std::any_of(
      orders_.begin(), orders_.end(),
      [&id](const SalesOrder &order) { return order.customerId() == id; });
  if (referenced) {
    std::cout << "Customer has sales orders; deletion is blocked.\n";
    return;
  }
  customers_.erase(it);
  std::cout << "Customer deleted.\n";
}

void Application::receiveProduct() {
  const std::string sku = readString("SKU: ");
  ProductRecord *record = findProduct(sku);
  if (!record) {
    std::cout << "Product not found.\n";
    return;
  }

  const int quantity = readInt("Quantity to receive: ");
  if (quantity <= 0) {
    std::cout << "Quantity must be positive.\n";
    return;
  }

  warehouse_.receive(quantity);
  record->stock.receive(quantity);
  std::cout << "Received " << quantity << " units of " << sku << ".\n";
}

void Application::dispatchProduct() {
  const std::string sku = readString("SKU: ");
  ProductRecord *record = findProduct(sku);
  if (!record) {
    std::cout << "Product not found.\n";
    return;
  }

  const int quantity = readInt("Quantity to dispatch: ");
  if (quantity <= 0 || !record->product.isAvailableForSale(
                           quantity, record->stock.available())) {
    std::cout << "Invalid quantity or insufficient available stock.\n";
    return;
  }

  record->stock.issue(quantity);
  warehouse_.dispatch(quantity);
  std::cout << "Dispatched " << quantity << " units of " << sku << ".\n";
}

void Application::createOrder() {
  const std::string orderId = readString("Order ID: ");
  if (orderId.empty() || std::any_of(orders_.begin(), orders_.end(),
                                     [&orderId](const SalesOrder &order) {
                                       return order.orderId() == orderId;
                                     })) {
    std::cout << "Order ID is empty or already exists.\n";
    return;
  }

  const std::string customerId = readString("Customer ID: ");
  if (!findCustomer(customerId)) {
    std::cout
        << "Customer not found. Add the customer to the directory first.\n";
    return;
  }
  SalesOrder order(orderId, customerId);

  while (true) {
    const std::string sku = readString("SKU (0 to finish): ");
    if (sku == "0")
      break;

    const ProductRecord *record = findProduct(sku);
    if (!record) {
      std::cout << "Product not found.\n";
      continue;
    }

    const int quantity = readInt("Quantity: ");
    if (!record->product.isAvailableForSale(quantity,
                                            record->stock.available())) {
      std::cout << "Invalid quantity or insufficient stock.\n";
      continue;
    }

    try {
      order.addLine(SalesOrderLine(sku, quantity, record->product.unitPrice()));
      std::cout << "Line added.\n";
    } catch (const std::exception &error) {
      std::cout << "Cannot add line: " << error.what() << '\n';
    }
  }

  if (order.lines().empty()) {
    std::cout << "Empty order was not saved.\n";
    return;
  }

  order.confirm();
  std::cout << "Order created. Total: " << order.calculateTotal() << '\n';
  orders_.push_back(std::move(order));
}

const char *Application::orderStatusName(SalesOrderStatus status) {
  switch (status) {
  case SalesOrderStatus::Draft:
    return "Draft";
  case SalesOrderStatus::Confirmed:
    return "Confirmed";
  case SalesOrderStatus::Picking:
    return "Picking";
  case SalesOrderStatus::Shipped:
    return "Shipped";
  case SalesOrderStatus::Cancelled:
    return "Cancelled";
  }
  return "Unknown";
}

void Application::showOrders() const {
  if (orders_.empty()) {
    std::cout << "No orders.\n";
    return;
  }

  for (const auto &order : orders_) {
    std::cout << "\nOrder: " << order.orderId()
              << " | Customer: " << order.customerId()
              << " | Status: " << orderStatusName(order.status())
              << " | Total: " << order.calculateTotal() << '\n';
    for (const auto &line : order.lines()) {
      std::cout << "  " << line.sku() << " x " << line.quantity() << " @ "
                << line.unitPrice() << " = " << line.lineTotal() << '\n';
    }
  }
}

void Application::shipOrder() {
  const std::string orderId = readString("Order ID: ");
  auto it = std::find_if(orders_.begin(), orders_.end(),
                         [&orderId](const SalesOrder &order) {
                           return order.orderId() == orderId;
                         });

  if (it == orders_.end()) {
    std::cout << "Order not found.\n";
    return;
  }
  if (it->status() != SalesOrderStatus::Confirmed) {
    std::cout << "Only confirmed orders can be shipped.\n";
    return;
  }

  int totalQuantity = 0;
  for (const auto &line : it->lines()) {
    const ProductRecord *record = findProduct(line.sku());
    if (!record || !record->product.isAvailableForSale(
                       line.quantity(), record->stock.available())) {
      std::cout << "Insufficient stock for " << line.sku()
                << ". Shipment cancelled.\n";
      return;
    }
    totalQuantity += line.quantity();
  }

  it->beginPicking();
  for (const auto &line : it->lines()) {
    ProductRecord *record = findProduct(line.sku());
    record->stock.issue(line.quantity());
  }
  warehouse_.dispatch(totalQuantity);
  it->markShipped();
  std::cout << "Order " << orderId << " shipped.\n";
}

void Application::showWarehouseInfo() const {
  std::cout << "\nWarehouse: " << warehouse_.code()
            << "\nAddress: " << warehouse_.address()
            << "\nCapacity: " << warehouse_.capacity()
            << "\nOccupied: " << warehouse_.occupied()
            << "\nFree: " << warehouse_.freeCapacity()
            << "\nOccupancy: " << std::fixed << std::setprecision(2)
            << warehouse_.occupancyRate() * 100.0 << "%"
            << "\nActive: " << (warehouse_.active() ? "yes" : "no")
            << "\nZones: ";

  for (const auto &zoneCode : warehouse_.zoneCodes()) {
    std::cout << zoneCode << ' ';
  }
  std::cout << '\n';
}

void Application::showSuppliers() const {
  std::cout << "\n========== SUPPLIERS ==========" << '\n';
  if (suppliers_.empty()) {
    std::cout << "No suppliers registered.\n";
    return;
  }

  for (const auto &supplier : suppliers_) {
    std::cout << "ID: " << supplier.supplierId()
              << " | Company: " << supplier.companyName()
              << " | Email: " << supplier.contactEmail()
              << " | Lead time: " << supplier.leadTimeDays() << " days"
              << " | Reliability: " << std::fixed << std::setprecision(2)
              << supplier.reliabilityScore() << "/5"
              << " | Active: " << (supplier.active() ? "yes" : "no") << '\n';

    for (const auto &contract : supplierContracts_) {
      if (contract.belongsTo(supplier)) {
        std::cout << "  Contract " << contract.contractId()
                  << " | Minimum order: " << contract.minimumOrderValue()
                  << " | Valid today: "
                  << (contract.isValidOn(currentDate()) ? "yes" : "no") << '\n';
      }
    }
  }
}

const char *
Application::purchaseRequestStatusName(PurchaseRequestStatus status) {
  switch (status) {
  case PurchaseRequestStatus::Draft:
    return "Draft";
  case PurchaseRequestStatus::Submitted:
    return "Submitted";
  case PurchaseRequestStatus::Approved:
    return "Approved";
  case PurchaseRequestStatus::Rejected:
    return "Rejected";
  case PurchaseRequestStatus::Cancelled:
    return "Cancelled";
  }
  return "Unknown";
}

const char *Application::purchaseOrderStatusName(PurchaseOrderStatus status) {
  switch (status) {
  case PurchaseOrderStatus::Draft:
    return "Draft";
  case PurchaseOrderStatus::Approved:
    return "Approved";
  case PurchaseOrderStatus::PartiallyReceived:
    return "Partially received";
  case PurchaseOrderStatus::Received:
    return "Received";
  case PurchaseOrderStatus::Cancelled:
    return "Cancelled";
  }
  return "Unknown";
}

void Application::createPurchaseRequest() {
  const std::string requestId = readString("Request ID: ");
  if (requestId.empty() || findPurchaseRequest(requestId)) {
    std::cout << "Request ID is empty or already exists.\n";
    return;
  }

  const std::string department = readString("Department code: ");
  const std::string requester = readString("Requester employee ID: ");
  if (!findEmployee(requester) && requester != warehouseManager_.employeeId()) {
    std::cout << "Requester employee was not found.\n";
    return;
  }
  PurchaseRequest request(requestId, department, requester);

  std::cout << "Add requested products. Enter 0 as SKU to finish.\n";
  while (true) {
    const std::string sku = readString("SKU: ");
    if (sku == "0")
      break;

    if (!findProduct(sku)) {
      std::cout << "Unknown SKU. Add the product to the catalog first.\n";
      continue;
    }

    const int quantity = readInt("Requested quantity: ");
    const double estimatedPrice =
        readNonNegativeDouble("Estimated unit price: ");
    if (!request.addLine({sku, quantity, estimatedPrice})) {
      std::cout << "Invalid line or request is no longer editable.\n";
    } else {
      std::cout << "Request line added.\n";
    }
  }

  if (!request.submit()) {
    std::cout << "Request was not submitted: it has no valid lines.\n";
    return;
  }

  const bool hasAvailableSupplier =
      std::any_of(suppliers_.begin(), suppliers_.end(),
                  [&request](const Supplier &supplier) {
                    return supplier.canFulfil(request);
                  });
  if (!hasAvailableSupplier) {
    std::cout << "Warning: no active supplier is currently available for this "
                 "request.\n";
  }

  std::cout << "Purchase request submitted. Estimated cost: "
            << request.estimatedCost() << '\n';
  purchaseRequests_.push_back(std::move(request));
}

void Application::approvePurchaseRequest() {
  const std::string requestId = readString("Request ID: ");
  PurchaseRequest *request = findPurchaseRequest(requestId);
  if (!request) {
    std::cout << "Purchase request not found.\n";
    return;
  }

  const std::string managerId =
      readString("Approving manager ID (EMP-MGR-01): ");
  if (managerId != warehouseManager_.employeeId()) {
    std::cout << "Manager not found or not authorized.\n";
    return;
  }
  if (request->departmentCode() != warehouseManager_.departmentCode()) {
    std::cout << "The manager cannot approve requests from this department.\n";
    return;
  }
  if (!warehouseManager_.approvePurchaseRequest(*request)) {
    std::cout << "Request cannot be approved: invalid state or approval limit "
                 "exceeded.\n";
    return;
  }
  std::cout << "Purchase request " << requestId << " approved by "
            << request->approverId()
            << ". Approval limit: " << warehouseManager_.approvalLimit()
            << ".\n";
}

void Application::createPurchaseOrder() {
  const std::string requestId = readString("Approved request ID: ");
  PurchaseRequest *request = findPurchaseRequest(requestId);
  if (!request || request->status() != PurchaseRequestStatus::Approved) {
    std::cout << "An approved purchase request was not found.\n";
    return;
  }

  const std::string supplierId = readString("Supplier ID (SUP-001): ");
  Supplier *supplier = findSupplier(supplierId);
  if (!supplier || !supplier->active()) {
    std::cout << "Supplier not found or inactive.\n";
    return;
  }

  const std::string today = currentDate();
  const auto contractIt = std::find_if(
      supplierContracts_.begin(), supplierContracts_.end(),
      [&supplierId, &today, supplier](const SupplierContract &contract) {
        return contract.supplierId() == supplierId &&
               contract.belongsTo(*supplier) && contract.isValidOn(today);
      });
  if (contractIt == supplierContracts_.end()) {
    std::cout << "No valid contract exists for this supplier.\n";
    return;
  }

  const std::string orderId = readString("Purchase order ID: ");
  if (orderId.empty() || findPurchaseOrder(orderId)) {
    std::cout << "Purchase order ID is empty or already exists.\n";
    return;
  }

  PurchaseOrder order(orderId, supplierId);
  for (const auto &line : request->lines()) {
    order.addLine(
        PurchaseOrderLine(line.sku, line.quantity, line.estimatedUnitPrice));
  }

  if (order.totalCost() < contractIt->minimumOrderValue()) {
    std::cout << "Order value is below the supplier contract minimum of "
              << contractIt->minimumOrderValue() << ".\n";
    return;
  }

  order.approve();
  std::cout << "Purchase order " << order.id()
            << " approved and sent to supplier.\n"
            << "Supplier: " << supplier->companyName() << '\n'
            << "Order total: " << order.totalCost() << '\n';
  purchaseOrders_.push_back(std::move(order));
}

void Application::receivePurchaseOrder() {
  const std::string orderId = readString("Purchase order ID: ");
  PurchaseOrder *order = findPurchaseOrder(orderId);
  if (!order) {
    std::cout << "Purchase order not found.\n";
    return;
  }
  if (order->status() != PurchaseOrderStatus::Approved &&
      order->status() != PurchaseOrderStatus::PartiallyReceived) {
    std::cout << "This purchase order cannot receive deliveries.\n";
    return;
  }

  Supplier *supplier = findSupplier(order->supplierId());
  if (!supplier) {
    std::cout << "Supplier record is missing.\n";
    return;
  }

  const std::string noteId = readString("Delivery note number: ");
  const std::string receiptId = readString("Goods receipt ID: ");
  const auto duplicateReceipt =
      std::find_if(receiptHistory_.begin(), receiptHistory_.end(),
                   [&receiptId](const ReceiptSummary &receipt) {
                     return receipt.receiptId == receiptId;
                   });
  const auto duplicateSessionReceipt =
      std::find_if(goodsReceipts_.begin(), goodsReceipts_.end(),
                   [&receiptId](const GoodsReceipt &receipt) {
                     return receipt.receiptId() == receiptId;
                   });
  const auto duplicateNote =
      std::find_if(deliveryNotes_.begin(), deliveryNotes_.end(),
                   [&noteId](const DeliveryNote &note) {
                     return note.noteNumber() == noteId;
                   });
  if (noteId.empty() || receiptId.empty() ||
      duplicateReceipt != receiptHistory_.end() ||
      duplicateSessionReceipt != goodsReceipts_.end() ||
      duplicateNote != deliveryNotes_.end()) {
    std::cout << "Delivery note/receipt ID is empty or already exists.\n";
    return;
  }

  const std::string receivedDate = readString("Received date (YYYY-MM-DD): ");
  if (parseDate(receivedDate) == static_cast<std::time_t>(-1)) {
    std::cout << "Invalid date. Use YYYY-MM-DD.\n";
    return;
  }
  if (!qualityInspector_.certificateValidOn(receivedDate)) {
    std::cout
        << "The quality inspector certificate is not valid on this date.\n";
    return;
  }

  const auto contractIt = std::find_if(
      supplierContracts_.begin(), supplierContracts_.end(),
      [&supplier, &receivedDate](const SupplierContract &contract) {
        return contract.belongsTo(*supplier) &&
               contract.isValidOn(receivedDate);
      });
  if (contractIt == supplierContracts_.end()) {
    std::cout << "No supplier contract is valid on the delivery date.\n";
    return;
  }

  const std::string punctuality =
      readString("Was the delivery on time? (y/n): ");
  if (punctuality != "y" && punctuality != "Y" && punctuality != "n" &&
      punctuality != "N") {
    std::cout << "Enter y or n.\n";
    return;
  }
  const bool onTime = punctuality == "y" || punctuality == "Y";
  int overdueDays = 0;
  if (!onTime) {
    overdueDays = readInt("Overdue days: ");
    if (overdueDays <= 0) {
      std::cout << "Overdue days must be positive.\n";
      return;
    }
  }

  DeliveryNote note(noteId, supplier->supplierId(), order->id());
  GoodsReceipt receipt(receiptId, order->id(), qualityInspector_.employeeId());

  struct AcceptedLine {
    std::string sku;
    int quantity;
    double unitCost;
    std::time_t expiry;
    bool qualityPassed;
  };
  std::vector<AcceptedLine> acceptedLines;
  int totalAccepted = 0;

  std::cout << "Enter delivered quantity for each outstanding order line.\n";
  for (const auto &orderLine : order->lines()) {
    if (orderLine.outstanding() == 0)
      continue;

    std::cout << "SKU " << orderLine.sku()
              << ", outstanding: " << orderLine.outstanding() << '\n';
    const int delivered = readInt("Delivered quantity (0 to skip): ");
    if (delivered < 0 || delivered > orderLine.outstanding()) {
      std::cout
          << "Quantity is outside the outstanding amount. Receipt cancelled.\n";
      return;
    }
    if (delivered == 0)
      continue;

    const int accepted =
        readInt("Quantity accepted after incoming inspection: ");
    if (accepted < 0 || accepted > delivered) {
      std::cout
          << "Accepted quantity must be between zero and delivered quantity.\n";
      return;
    }

    const int rejected = delivered - accepted;
    if (rejected > 0) {
      const std::string reason = readString("Rejection reason: ");
      if (!receipt.rejectLine(orderLine.sku(), rejected, reason)) {
        std::cout << "Could not register rejected quantity.\n";
        return;
      }
    }

    if (accepted == 0)
      continue;

    const std::string expiryDate =
        readString("Expiry date for accepted lot (YYYY-MM-DD): ");
    const std::time_t expiry = parseDate(expiryDate);
    if (expiry == static_cast<std::time_t>(-1)) {
      std::cout << "Invalid expiry date. Receipt cancelled.\n";
      return;
    }

    const std::string qualityAnswer = readString("Quality passed? (y/n): ");
    const bool qualityPassed = qualityAnswer == "y" || qualityAnswer == "Y";
    if (!qualityPassed && qualityAnswer != "n" && qualityAnswer != "N") {
      std::cout << "Enter y or n. Receipt cancelled.\n";
      return;
    }

    if (!note.addLine({orderLine.sku(), accepted})) {
      std::cout << "Could not add delivery note line.\n";
      return;
    }

    const double unitCost =
        orderLine.lineTotal() / static_cast<double>(orderLine.ordered());
    acceptedLines.push_back(
        {orderLine.sku(), accepted, unitCost, expiry, qualityPassed});
    totalAccepted += accepted;
  }

  if (note.lines().empty()) {
    // The domain API requires at least one accepted line to complete a
    // DeliveryNote. Preserve the rejected-only receipt as an incomplete record;
    // the PO remains open.
    deliveryNotes_.push_back(std::move(note));
    receiptHistory_.push_back(
        {receipt.receiptId(), receipt.purchaseOrderId(), receipt.inspectorId(),
         receipt.acceptedQuantity(), receipt.rejectedQuantity(),
         receipt.completed(), receipt.rejectionReason()});
    goodsReceipts_.push_back(std::move(receipt));
    if (onTime)
      supplier->recordOnTimeDelivery();
    else
      supplier->recordLateDelivery();
    std::cout
        << "All delivered goods were rejected. The rejection record was saved; "
        << "the purchase order remains outstanding.\n";
    if (!onTime) {
      std::cout << "Contract penalty: "
                << contractIt->calculatePenalty(overdueDays, order->totalCost())
                << '\n';
    }
    return;
  }

  if (!note.validateAgainstOrder(*order)) {
    std::cout
        << "Delivery note does not match the outstanding purchase order.\n";
    return;
  }

  // Reserve warehouse capacity before mutating the purchase order.
  warehouse_.receive(totalAccepted);
  if (!receipt.acceptDelivery(note, *order, receivedDate)) {
    warehouse_.dispatch(totalAccepted);
    std::cout << "Goods receipt could not be completed.\n";
    return;
  }

  for (const auto &line : acceptedLines) {
    const std::string lotId = receiptId + "-" + line.sku;
    StockLot lot(lotId, line.sku, line.quantity, line.expiry, line.unitCost);
    const bool passedQuality = line.qualityPassed && !lot.isExpired();
    qualityInspector_.inspectLot(lot, passedQuality);

    if (passedQuality) {
      ProductRecord *product = findProduct(line.sku);
      if (product) {
        product->stock.receive(line.quantity);
      }
      std::cout << "Lot " << lot.lotNumber()
                << " passed quality control and was added to stock.\n";
    } else if (lot.isExpired()) {
      std::cout << "Lot " << lot.lotNumber()
                << " is expired and was quarantined.\n";
    } else {
      std::cout << "Lot " << lot.lotNumber()
                << " failed quality control and was quarantined.\n";
    }
    stockLots_.push_back(std::move(lot));
  }

  if (onTime)
    supplier->recordOnTimeDelivery();
  else
    supplier->recordLateDelivery();
  std::cout << "Goods receipt completed. Accepted: "
            << receipt.acceptedQuantity()
            << ", rejected: " << receipt.rejectedQuantity()
            << ", accepted value: " << receipt.calculateAcceptedValue(*order)
            << '\n';
  if (!onTime) {
    std::cout << "Contract penalty: "
              << contractIt->calculatePenalty(overdueDays, order->totalCost())
              << '\n';
  }

  deliveryNotes_.push_back(std::move(note));
  receiptHistory_.push_back({receipt.receiptId(), receipt.purchaseOrderId(),
                             receipt.inspectorId(), receipt.acceptedQuantity(),
                             receipt.rejectedQuantity(), receipt.completed(),
                             receipt.rejectionReason()});
  goodsReceipts_.push_back(std::move(receipt));
}

void Application::showProcurement() const {
  std::cout << "\n========== PURCHASE REQUESTS ==========" << '\n';
  if (purchaseRequests_.empty())
    std::cout << "No purchase requests.\n";
  for (const auto &request : purchaseRequests_) {
    std::cout << request.requestId()
              << " | Department: " << request.departmentCode()
              << " | Requester: " << request.requesterId()
              << " | Status: " << purchaseRequestStatusName(request.status())
              << " | Estimated cost: " << request.estimatedCost() << '\n';
    for (const auto &line : request.lines()) {
      std::cout << "  " << line.sku << " x " << line.quantity << " @ "
                << line.estimatedUnitPrice << '\n';
    }
  }

  std::cout << "\n========== PURCHASE ORDERS ==========" << '\n';
  if (purchaseOrders_.empty())
    std::cout << "No purchase orders.\n";
  for (const auto &order : purchaseOrders_) {
    std::cout << order.id() << " | Supplier: " << order.supplierId()
              << " | Status: " << purchaseOrderStatusName(order.status())
              << " | Total: " << order.totalCost() << '\n';
    for (const auto &line : order.lines()) {
      std::cout << "  " << line.sku() << " | ordered: " << line.ordered()
                << " | received: " << line.received()
                << " | outstanding: " << line.outstanding() << '\n';
    }
  }

  std::cout << "\n========== GOODS RECEIPTS ==========" << '\n';
  if (receiptHistory_.empty())
    std::cout << "No goods receipts.\n";
  for (const auto &receipt : receiptHistory_) {
    std::cout << receipt.receiptId << " | PO: " << receipt.purchaseOrderId
              << " | Inspector: " << receipt.inspectorId
              << " | Accepted: " << receipt.acceptedQuantity
              << " | Rejected: " << receipt.rejectedQuantity
              << " | Completed: " << (receipt.completed ? "yes" : "no")
              << " | Reason: " << receipt.rejectionReason << '\n';
  }

  std::cout << "\n========== STOCK LOTS ==========" << '\n';
  if (stockLots_.empty())
    std::cout << "No stock lots.\n";
  for (const auto &lot : stockLots_) {
    std::cout << lot.lotNumber() << " | SKU: " << lot.sku()
              << " | Quantity: " << lot.quantity()
              << " | Cost: " << lot.totalCost()
              << " | Quarantined: " << (lot.quarantined() ? "yes" : "no")
              << " | Expired: " << (lot.isExpired() ? "yes" : "no") << '\n';
  }

  std::cout << "\nQuality inspector " << qualityInspector_.employeeId()
            << " | approved lots: " << qualityInspector_.approvedLots()
            << " | rejected lots: " << qualityInspector_.rejectedLots() << '\n';
}
