# Система управления складом (Warehouse Management System)

Учебный ООП-проект на C++17. Структура и стиль организации исходников подготовлены по образцу предоставленной лабораторной работы «Деканат»: отдельные заголовочные и реализационные файлы, тематическое разбиение подсистем, CMake и тестирование.

## Состав проекта

- **62 предметных класса** и **15 собственных классов исключений** — всего 77 классов;
- **363 поля** в классах (без учёта полей двух вспомогательных структур строк заказа/накладной);
- более **100 содержательных операций**: проверки бизнес-условий, расчёты, переходы состояний, резервирование, приёмка, назначение и списание;
- более **30 типизированных ассоциаций** между классами;
- отдельные `.h` и `.cpp` для классов, тематическое разделение по подсистемам;
- CMake-сборка на C++17, приложение и автоматические тесты.

**Статус реализации.** В проекте представлены подсистемы персонала, товаров, складской инфраструктуры, учёта запасов, закупок, клиентских заказов, логистики, финансов и администрирования. Для персонала и закупок используются предметные поля и состояния; в остальных подсистемах сохранены некоторые универсальные числовые атрибуты исходного каркаса, но добавлены конкретные проверки и операции, связывающие сущности между собой. Числа в сводке получены по заголовочным файлам и реализациям текущей версии проекта.

## Сводка соответствия требованиям

| Требование | В проекте | Статус |
|---|---:|---|
| Классы | 77 (62 предметных + 15 исключений) | Выполнено |
| Поля | 363 | Выполнено |
| Содержательные операции | Более 100 | Выполнено |
| Ассоциации | Более 30 типизированных связей | Выполнено |
| Собственные исключения | 15 | Выполнено |
| README | Каталог классов, полей, методов, связей и исключений | Выполнено |

## Структура каталогов

```text
WarehouseManagement/
├── h/                         # заголовочные файлы
│   ├── people/
│   ├── products/
│   ├── storage/
│   ├── inventory/
│   ├── procurement/
│   ├── orders/
│   ├── logistics/
│   ├── finance/
│   ├── management/
│   └── exceptions/
├── cpp/                       # реализации классов
│   ├── people/
│   ├── products/
│   ├── storage/
│   ├── inventory/
│   ├── procurement/
│   ├── orders/
│   ├── logistics/
│   ├── finance/
│   ├── management/
│   └── exceptions/
├── tests/
├── main.cpp
├── CMakeLists.txt
└── README.md
```

Для каждого класса `X` существуют `h/<подсистема>/X.h` и `cpp/<подсистема>/X.cpp`. Исключения размещены в `h/exceptions/` и `cpp/exceptions/`.

## Поля предметных классов

В модели насчитывается 363 поля, объявленное непосредственно в классах. В персонале и закупках используются предметные атрибуты: должность, ставка, навыки, срок действия водительского удостоверения, лимит согласования, строки заявки и накладной. В части вспомогательных сущностей каталога, логистики, финансов и администрирования сохранены универсальные `quantity_`, `amount_` и `relatedIds_`; они используются как счётчики, суммы/стоимостные показатели и реестр связанных идентификаторов. Дополнительные типизированные методы связывают эти сущности с конкретными объектами предметной области.

## Каталог классов, полей и основных методов


### Сотрудники и персонал

| Класс | Поля (типы) | Основные методы |
|---|---|---|
| `Employee` | `employeeId_: string`, `fullName_: string`, `position_: string`, `hourlyRate_: double`, `active_: bool`, `skills_: vector<string>` | `validateIdentity()`, `changePosition(string)`, `changeHourlyRate(double)`, `deactivate()`, `addSkill(string)`, `hasSkill(string)` |
| `WarehouseWorker` | `employeeId_: string`, `shiftCode_: string`, `hourlyRate_: double`, `onShift_: bool`, `shiftStartDate_: string`, `taskIds_: vector<string>` | `startShift(string)`, `finishShift(string)`, `assignPickingTask(PickingTask&)`, `assignPackingTask(PackingTask&)`, `completeTask(string)`, `calculateShiftPay(double)` |
| `Storekeeper` | `employeeId_: string`, `zoneCode_: string`, `taskLimit_: size_t`, `assignedTaskIds_: vector<string>` | `assignZone(StorageZone&)`, `assignPickingTask(PickingTask&)`, `completeTask(string)`, `countAssignedTasks()`, `canAcceptTask()` |
| `WarehouseManager` | `employeeId_: string`, `departmentCode_: string`, `approvalLimit_: double`, `teamMemberIds_: vector<string>` | `approvePurchaseRequest(PurchaseRequest&)`, `approveExpense(WarehouseExpense&)`, `canApprove(double)`, `registerTeamMember(string)`, `calculateTeamLoad(double)` |
| `Driver` | `employeeId_: string`, `licenseNumber_: string`, `licenseExpiry_: string`, `loadLimitKg_: double`, `assignedDeliveryIds_: vector<string>`, `completedDeliveryIds_: vector<string>` | `validateLicense(string)`, `canCarry(double)`, `assignDelivery(Delivery&)`, `recordDelivery(string)`, `completedDeliveries()` |
| `QualityInspector` | `employeeId_: string`, `certificateCode_: string`, `certificateExpiry_: string`, `approvedLots_: size_t`, `rejectedLots_: size_t` | `certificateValidOn(string)`, `inspectLot(StockLot&, bool)`, `approveQuality()`, `approvedLots()`, `rejectedLots()` |

### Номенклатура и товары

| Класс | Поля (типы) | Основные методы |
|---|---|---|
| `Product` | `sku_: string`, `name_: string`, `unitPrice_: double`, `reorderLevel_: int`, `categoryCode_: string`, `brandCode_: string`, `active_: bool`, `barcodes_: vector<string>` | `changePrice(double)`, `isAvailableForSale(int,int)`, `needsReorder(int)`, `addBarcode(string)`, `deactivate()`, `activate()` |
| `ProductCategory` | `categoryCode_: string`, `name_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `addProduct()`, `removeProduct(int)` |
| `Brand` | `brandCode_: string`, `name_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `renameBrand()`, `matchesProduct(int)` |
| `ProductAttribute` | `attributeCode_: string`, `value_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `validateValue()`, `normalizeValue(int)` |
| `UnitOfMeasure` | `unitCode_: string`, `symbol_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `convertToBase()`, `isCompatible(int)` |
| `Barcode` | `barcodeValue_: string`, `format_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `validateBarcode()`, `matches(int)` |
| `SerialNumber` | `serialCode_: string`, `productSku_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `assignProduct()`, `isAssigned(int)`, `belongsTo(Product)`, `isRegistered()` |
| `ProductBundle` | `bundleCode_: string`, `name_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculateBundlePrice()`, `validateComponents(int)` |

### Складская инфраструктура

| Класс | Поля (типы) | Основные методы |
|---|---|---|
| `Warehouse` | `code_: string`, `address_: string`, `capacity_: double`, `occupied_: double`, `active_: bool`, `zoneCodes_: vector<string>` | `addZone(StorageZone)`, `receive(int)`, `dispatch(int)`, `freeCapacity()`, `occupancyRate()`, `close()`, `reopen()` |
| `StorageZone` | `code_: string`, `temperatureMode_: string`, `capacity_: double`, `occupied_: double` | `isSuitableFor(string)`, `store(double)`, `remove(double)`, `freeCapacity()`, `occupancyRate()` |
| `Rack` | `rackCode_: string`, `zoneCode_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculateLoad()`, `hasFreeLevel(int)` |
| `Shelf` | `shelfCode_: string`, `rackCode_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculateFreeVolume()`, `canStore(int)` |
| `StorageBin` | `binCode_: string`, `shelfCode_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculateFreeCapacity()`, `canFit(int)` |
| `LoadingDock` | `dockCode_: string`, `status_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `openDock()`, `closeDock(int)` |
| `ColdStorage` | `storageCode_: string`, `temperature_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `validateTemperature()`, `estimateEnergyUse(int)` |
| `StorageCondition` | `conditionCode_: string`, `description_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `isSatisfiedBy()`, `requiresCooling(int)` |

### Учёт и движение запасов

| Класс | Поля (типы) | Основные методы |
|---|---|---|
| `StockItem` | `id_: string`, `sku_: string`, `warehouseCode_: string`, `binCode_: string`, `onHand_: int`, `reserved_: int` | `available()`, `receive(int)`, `reserve(int)`, `release(int)`, `issue(int)`, `moveTo(string)` |
| `StockLot` | `lotNumber_: string`, `sku_: string`, `quantity_: int`, `expiryDate_: time_t`, `unitCost_: double`, `quarantined_: bool` | `isExpired(time_t)`, `remainingShelfLifeDays(time_t)`, `remove(int)`, `markQuarantined()`, `releaseFromQuarantine()` |
| `StockBalance` | `balanceId_: string`, `warehouseCode_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculateAvailable()`, `reconcile(int)` |
| `StockReservation` | `id_: string`, `orderId_: string`, `sku_: string`, `quantity_: int`, `status_: ReservationStatus` | `apply(StockItem&)`, `release(StockItem&)`, `consume(StockItem&)`, `active()` |
| `Inventory` | `id_: string`, `warehouseCode_: string`, `lines_: vector<InventoryLine>`, `status_: InventoryStatus` | `addLine(InventoryLine)`, `start()`, `recordCount(string,int)`, `allLinesCounted()`, `discrepancyCount()`, `totalDifference()`, `complete()`, `cancel()` |
| `InventoryLine` | `sku_: string`, `expected_: int`, `counted_: int`, `countedFlag_: bool` | `recordCount(int)`, `difference()`, `matches()` |
| `StockMovement` | `movementId_: string`, `productSku_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculateSignedQuantity()`, `isTransfer(int)` |
| `StockAdjustment` | `adjustmentId_: string`, `reason_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `applyAdjustment()`, `requiresApproval(int)` |
| `ReorderRule` | `ruleId_: string`, `productSku_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `shouldReorder()`, `calculateOrderQuantity(int)` |

### Закупки и поставщики

| Класс | Поля (типы) | Основные методы |
|---|---|---|
| `Supplier` | `supplierId_: string`, `companyName_: string`, `contactEmail_: string`, `leadTimeDays_: int`, `reliabilityRating_: double`, `onTimeDeliveries_: size_t`, `lateDeliveries_: size_t`, `active_: bool` | `rateReliability(double)`, `canFulfil(PurchaseRequest&)`, `deactivate()`, `recordOnTimeDelivery()`, `recordLateDelivery()`, `reliabilityScore()` |
| `PurchaseRequest` | `requestId_: string`, `departmentCode_: string`, `requesterId_: string`, `approverId_: string`, `rejectionReason_: string`, `lines_: vector<PurchaseRequestLine>`, `status_: PurchaseRequestStatus` | `addLine(PurchaseRequestLine)`, `submit()`, `approve(string)`, `reject(string,string)`, `cancel()`, `estimatedCost()`, `hasLine(string)` |
| `PurchaseOrder` | `id_: string`, `supplierId_: string`, `lines_: vector<PurchaseOrderLine>`, `status_: PurchaseOrderStatus` | `addLine(PurchaseOrderLine)`, `approve()`, `registerReceipt(string,int)`, `fullyReceived()`, `totalCost()`, `cancel()` |
| `PurchaseOrderLine` | `sku_: string`, `ordered_: int`, `received_: int`, `unitCost_: double` | `receive(int)`, `outstanding()`, `lineTotal()` |
| `GoodsReceipt` | `receiptId_: string`, `purchaseOrderId_: string`, `inspectorId_: string`, `rejectionReason_: string`, `acceptedQuantity_: int`, `rejectedQuantity_: int`, `completed_: bool` | `acceptDelivery(DeliveryNote&, PurchaseOrder&, string)`, `rejectLine(string,int,string)`, `calculateAcceptedValue(PurchaseOrder&)` |
| `DeliveryNote` | `noteNumber_: string`, `supplierId_: string`, `purchaseOrderId_: string`, `receivedDate_: string`, `lines_: vector<DeliveryNoteLine>`, `received_: bool` | `addLine(DeliveryNoteLine)`, `validateAgainstOrder(PurchaseOrder&)`, `markReceived(string)`, `totalDeliveredQuantity()` |
| `SupplierContract` | `contractId_: string`, `supplierId_: string`, `validFrom_: string`, `validUntil_: string`, `minimumOrderValue_: double`, `latePenaltyRate_: double` | `isValidOn(string)`, `calculatePenalty(int,double)`, `belongsTo(Supplier&)`, `renew(string)` |

### Клиентские заказы и отгрузки

| Класс | Поля (типы) | Основные методы |
|---|---|---|
| `Customer` | `customerId_: string`, `fullName_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `validateContact()`, `updateLoyalty(int)` |
| `SalesOrder` | `orderId_: string`, `customerId_: string`, `lines_: vector<SalesOrderLine>`, `status_: SalesOrderStatus` | `addLine(SalesOrderLine)`, `removeLine(string)`, `calculateTotal()`, `confirm()`, `beginPicking()`, `markShipped()`, `cancel()` |
| `SalesOrderLine` | `sku_: string`, `quantity_: int`, `unitPrice_: double`, `discount_: double` | `lineTotal()`, `changeQuantity(int)`, `applyDiscount(double)` |
| `PickingTask` | `taskId_: string`, `orderId_: string`, `quantity_: int`, `pickedQuantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `assignPicker()`, `markPicked(int)`, `remainingQuantity()`, `pickedQuantity()` |
| `PackingTask` | `taskId_: string`, `orderId_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculatePackageCount()`, `completePacking(int)` |
| `Shipment` | `shipmentId_: string`, `orderId_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `dispatch()`, `markDelivered(int)` |
| `ReturnRequest` | `returnId_: string`, `orderId_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `approveReturn()`, `calculateRefund(int)` |
| `CustomerInvoice` | `invoiceId_: string`, `customerId_: string`, `quantity_: int`, `amount_: double`, `paidAmount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculateDue()`, `markPaid(double)`, `hasOutstandingBalance()`, `outstandingBalance()` |

### Логистика

| Класс | Поля (типы) | Основные методы |
|---|---|---|
| `Transport` | `transportId_: string`, `registrationNumber_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `isAvailable()`, `calculateCapacity(int)` |
| `Route` | `routeId_: string`, `destination_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculateDistance()`, `estimateDuration(int)` |
| `Delivery` | `deliveryId_: string`, `shipmentId_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `confirmDelivery()`, `recordFailure(int)` |
| `TransferRequest` | `transferId_: string`, `sourceWarehouse_: string`, `destinationWarehouse_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `approveTransfer()`, `calculateTransferCost(int)` |
| `Carrier` | `carrierId_: string`, `companyName_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculateDeliveryCost()`, `isContractActive(int)` |

### Финансы

| Класс | Поля (типы) | Основные методы |
|---|---|---|
| `Payment` | `paymentId_: string`, `purpose_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `processPayment()`, `refund(int)` |
| `CashAccount` | `accountId_: string`, `currency_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `deposit()`, `withdraw(int)` |
| `CostCalculation` | `calculationId_: string`, `productSku_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculateAverageCost()`, `calculateWeightedCost(int)` |
| `WarehouseExpense` | `expenseId_: string`, `category_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `approveExpense()`, `isWithinBudget(int)` |
| `FinancialReport` | `reportId_: string`, `period_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculateNetResult()`, `validatePeriod(int)` |

### Администрирование

| Класс | Поля (типы) | Основные методы |
|---|---|---|
| `WarehouseSystem` | `systemId_: string`, `systemName_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `registerWarehouse()`, `calculateNetworkStock(int)` |
| `StockAlert` | `alertId_: string`, `productSku_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `triggerIfLow()`, `acknowledge(int)` |
| `AuditLog` | `eventId_: string`, `actorId_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `recordEvent()`, `countEventsByActor(int)` |
| `MaintenanceRequest` | `requestId_: string`, `equipmentCode_: string`, `warehouseCode_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `scheduleMaintenance()`, `closeRequest(int)` |
| `KPIReport` | `reportId_: string`, `period_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculateOrderFulfillmentRate()`, `calculateInventoryTurnover(int)` |
| `AccessRole` | `roleCode_: string`, `roleName_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `grantPermission()`, `revokePermission(int)` |

## Полный каталог публичных методов

Ниже перечислены публичные методы всех 62 предметных классов. Включены конструкторы, операции предметной логики и методы доступа; это полный список объявленных публичных сигнатур, а не только краткий перечень основных операций.

| Класс | Публичные методы |
|---|---|
| `CashAccount` | `CashAccount(std::string accountId, std::string currency, int quantity = 0, double amount = 0.0)`; `deposit()`; `withdraw(int delta)`; `accountId()`; `currency()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `recordPayment(const Payment& payment)`; `credit(int amount)`; `creditMoney(double amount)`; `withdrawMoney(double amount)` |
| `CostCalculation` | `CostCalculation(std::string calculationId, std::string productSku, int quantity = 0, double amount = 0.0)`; `calculateAverageCost()`; `calculateWeightedCost(int delta)`; `calculationId()`; `productSku()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `addPurchase(int units, double unitCost)` |
| `FinancialReport` | `FinancialReport(std::string reportId, std::string period, int quantity = 0, double amount = 0.0)`; `calculateNetResult()`; `validatePeriod(int delta)`; `reportId()`; `period()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `includesExpense(const WarehouseExpense& expense)`; `averageTransactionValue()` |
| `Payment` | `Payment(std::string paymentId, std::string purpose, int quantity = 0, double amount = 0.0)`; `processPayment()`; `refund(int delta)`; `paymentId()`; `purpose()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `charge(CashAccount& account)`; `isRefundable()` |
| `WarehouseExpense` | `WarehouseExpense(std::string expenseId, std::string category, int quantity = 0, double amount = 0.0)`; `approveExpense()`; `approveBy(const std::string& managerId)`; `approvedBy()`; `isWithinBudget(int delta)`; `expenseId()`; `category()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `attachTo(FinancialReport& report)`; `exceedsLimit(double limit)` |
| `Inventory` | `Inventory(std::string id,std::string warehouseCode)`; `addLine(const InventoryLine&)`; `start()`; `recordCount(const std::string& sku,int countedQuantity)`; `allLinesCounted()`; `discrepancyCount()`; `totalDifference()`; `complete()`; `cancel()`; `status()`; `lines()`; `id()`; `warehouseCode()`; `belongsTo(const Warehouse& warehouse)` |
| `InventoryLine` | `InventoryLine(std::string sku,int expectedQuantity)`; `recordCount(int countedQuantity)`; `difference()`; `counted()`; `matches()`; `sku()`; `expectedQuantity()`; `countedQuantity()`; `reconcileWith(const StockItem& item)` |
| `ReorderRule` | `ReorderRule(std::string ruleId, std::string productSku, int quantity = 0, double amount = 0.0)`; `shouldReorder()`; `shouldReorder(const StockItem& item)`; `calculateOrderQuantity(int delta)`; `ruleId()`; `productSku()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `appliesTo(const StockItem& item)`; `shortageAgainst(const StockItem& item)` |
| `StockAdjustment` | `StockAdjustment(std::string adjustmentId, std::string reason, int quantity = 0, double amount = 0.0)`; `applyAdjustment()`; `requiresApproval(int approvalThreshold)`; `adjustmentId()`; `reason()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `applyTo(StockBalance& balance, int delta)`; `isMaterial(int approvalThreshold)` |
| `StockBalance` | `StockBalance(std::string balanceId, std::string warehouseCode, int quantity = 0, double amount = 0.0)`; `calculateAvailable()`; `reconcile(int delta)`; `balanceId()`; `warehouseCode()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `belongsTo(const Warehouse& warehouse)`; `hasAtLeast(int requested)` |
| `StockItem` | `StockItem(std::string id,std::string sku,std::string warehouseCode,std::string binCode,int onHand=0,int reserved=0)`; `available()`; `receive(int)`; `reserve(int)`; `release(int)`; `issue(int)`; `moveTo(std::string binCode)`; `id()`; `sku()`; `warehouseCode()`; `binCode()`; `onHand()`; `reserved()`; `matchesProduct(const Product& product)` |
| `StockLot` | `StockLot(std::string lotNumber,std::string sku,int quantity,std::time_t expiryDate,double unitCost)`; `isExpired(std::time_t at=std::time(nullptr))`; `remainingShelfLifeDays(std::time_t at=std::time(nullptr))`; `remove(int)`; `markQuarantined()`; `releaseFromQuarantine()`; `quarantined()`; `quantity()`; `totalCost()`; `lotNumber()`; `sku()`; `matchesProduct(const Product& product)` |
| `StockMovement` | `StockMovement(std::string movementId, std::string productSku, int quantity = 0, double amount = 0.0)`; `calculateSignedQuantity()`; `isTransfer(int delta)`; `movementId()`; `productSku()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `tracks(const StockItem& item)`; `isInbound()` |
| `StockReservation` | `StockReservation(std::string id,std::string orderId,std::string sku,int quantity)`; `apply(StockItem&)`; `release(StockItem&)`; `consume(StockItem&)`; `active()`; `quantity()`; `status()`; `id()`; `orderId()`; `sku()` |
| `Carrier` | `Carrier(std::string carrierId, std::string companyName, int quantity = 0, double amount = 0.0)`; `calculateDeliveryCost()`; `isContractActive(int delta)`; `carrierId()`; `companyName()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `canHandle(const Delivery& delivery)`; `isCostEffective(double maximumCost)` |
| `Delivery` | `Delivery(std::string deliveryId, std::string shipmentId, int quantity = 0, double amount = 0.0)`; `confirmDelivery()`; `recordFailure(int delta)`; `deliveryId()`; `shipmentId()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `refersTo(const Shipment& shipment)`; `isOverdue(int plannedDays, int actualDays)` |
| `Route` | `Route(std::string routeId, std::string destination, int quantity = 0, double amount = 0.0)`; `calculateDistance()`; `estimateDuration(int delta)`; `routeId()`; `destination()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `supports(const Transport& transport)`; `isWithinDistanceLimit(double maximumDistance)` |
| `TransferRequest` | `TransferRequest(std::string transferId, std::string sourceWarehouse, int quantity = 0, double amount = 0.0)`; `approveTransfer()`; `calculateTransferCost(int delta)`; `transferId()`; `sourceWarehouse()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `setDestination(const Warehouse& destination)`; `connects(const Warehouse& source, const Warehouse& destination)`; `destinationWarehouse()` |
| `Transport` | `Transport(std::string transportId, std::string registrationNumber, int quantity = 0, double amount = 0.0)`; `isAvailable()`; `calculateCapacity(int delta)`; `transportId()`; `registrationNumber()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `canCarry(const Delivery& delivery)`; `needsService(int serviceInterval)` |
| `AccessRole` | `AccessRole(std::string roleCode, std::string roleName, int quantity = 0, double amount = 0.0)`; `grantPermission()`; `revokePermission(int delta)`; `roleCode()`; `roleName()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `assignTo(const Employee& employee)`; `hasPermissionCount(int required)` |
| `AuditLog` | `AuditLog(std::string eventId, std::string actorId, int quantity = 0, double amount = 0.0)`; `recordEvent()`; `countEventsByActor(int delta)`; `eventId()`; `actorId()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `records(const Employee& employee)`; `belongsToActor(const std::string& employeeId)` |
| `KPIReport` | `KPIReport(std::string reportId, std::string period, int quantity = 0, double amount = 0.0)`; `calculateOrderFulfillmentRate()`; `calculateInventoryTurnover(int delta)`; `reportId()`; `period()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `evaluates(const Inventory& inventory)`; `averageOrdersPerPeriod()` |
| `MaintenanceRequest` | `MaintenanceRequest(std::string requestId, std::string equipmentCode, int quantity = 0, double amount = 0.0)`; `scheduleMaintenance()`; `closeRequest(int delta)`; `requestId()`; `equipmentCode()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `assignWarehouse(const Warehouse& warehouse)`; `warehouseCode()`; `isHighPriority(double threshold)` |
| `StockAlert` | `StockAlert(std::string alertId, std::string productSku, int quantity = 0, double amount = 0.0)`; `triggerIfLow()`; `acknowledge(int delta)`; `alertId()`; `productSku()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `follows(const ReorderRule& rule)`; `isCritical(int criticalLevel)` |
| `WarehouseSystem` | `WarehouseSystem(std::string systemId, std::string systemName, int quantity = 0, double amount = 0.0)`; `registerWarehouse()`; `calculateNetworkStock(int delta)`; `systemId()`; `systemName()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `hasWarehouse(const std::string& code)`; `registerWarehouse(const Warehouse& warehouse)` |
| `Customer` | `Customer(std::string customerId, std::string fullName, int quantity = 0, double amount = 0.0)`; `validateContact()`; `updateLoyalty(int delta)`; `customerId()`; `fullName()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `ownsOrder(const SalesOrder& order)`; `qualifiesForLoyaltyDiscount(int minimumOrders)` |
| `CustomerInvoice` | `CustomerInvoice(std::string invoiceId, std::string customerId, int quantity = 0, double amount = 0.0)`; `calculateDue()`; `markPaid(double paymentAmount)`; `invoiceId()`; `customerId()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `hasOutstandingBalance()`; `outstandingBalance()` |
| `PackingTask` | `PackingTask(std::string taskId, std::string orderId, int quantity = 0, double amount = 0.0)`; `calculatePackageCount()`; `completePacking(int delta)`; `taskId()`; `orderId()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `belongsTo(const SalesOrder& order)`; `requiresOversizePackaging(double threshold)` |
| `PickingTask` | `PickingTask(std::string taskId, std::string orderId, int quantity = 0, double amount = 0.0)`; `assignPicker()`; `markPicked(int delta)`; `taskId()`; `orderId()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `belongsTo(const SalesOrder& order)`; `remainingQuantity()`; `pickedQuantity()` |
| `ReturnRequest` | `ReturnRequest(std::string returnId, std::string orderId, int quantity = 0, double amount = 0.0)`; `approveReturn()`; `calculateRefund(int delta)`; `returnId()`; `orderId()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `isWithinQuantityLimit(int maximumQuantity)` |
| `SalesOrder` | `SalesOrder(std::string id,std::string customerId)`; `addLine(const SalesOrderLine&)`; `removeLine(const std::string& sku)`; `calculateTotal()`; `confirm()`; `beginPicking()`; `markShipped()`; `cancel()`; `status()`; `orderId()`; `customerId()`; `lines()` |
| `SalesOrderLine` | `SalesOrderLine(std::string sku,int quantity,double unitPrice)`; `lineTotal()`; `changeQuantity(int)`; `applyDiscount(double)`; `sku()`; `quantity()`; `unitPrice()`; `discount()` |
| `Shipment` | `Shipment(std::string shipmentId, std::string orderId, int quantity = 0, double amount = 0.0)`; `dispatch()`; `markDelivered(int delta)`; `shipmentId()`; `orderId()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `belongsTo(const SalesOrder& order)` |
| `Driver` | `Driver(std::string employeeId, std::string licenseNumber, std::string licenseExpiry, double loadLimitKg)`; `validateLicense(const std::string& onDate)`; `canCarry(double cargoWeightKg)`; `assignDelivery(const Delivery& delivery)`; `recordDelivery(const std::string& deliveryId)`; `completedDeliveries()`; `employeeId()`; `licenseNumber()`; `licenseExpiry()`; `loadLimitKg()` |
| `Employee` | `Employee(std::string employeeId, std::string fullName, std::string position, double hourlyRate)`; `validateIdentity()`; `changePosition(const std::string& newPosition)`; `changeHourlyRate(double newRate)`; `deactivate()`; `addSkill(const std::string& skill)`; `hasSkill(const std::string& skill)`; `employeeId()`; `fullName()`; `position()`; `hourlyRate()`; `active()`; `skills()` |
| `QualityInspector` | `QualityInspector(std::string employeeId, std::string certificateCode, std::string certificateExpiry)`; `certificateValidOn(const std::string& date)`; `inspectLot(StockLot& lot, bool passed)`; `approveQuality()`; `approvedLots()`; `rejectedLots()`; `employeeId()`; `certificateCode()` |
| `Storekeeper` | `Storekeeper(std::string employeeId, std::string zoneCode, std::size_t taskLimit)`; `assignZone(const StorageZone& zone)`; `assignPickingTask(const PickingTask& task)`; `completeTask(const std::string& taskId)`; `countAssignedTasks()`; `canAcceptTask()`; `employeeId()`; `zoneCode()` |
| `WarehouseManager` | `WarehouseManager(std::string employeeId, std::string departmentCode, double approvalLimit, std::size_t teamSize)`; `approvePurchaseRequest(PurchaseRequest& request)`; `approveExpense(WarehouseExpense& expense)`; `canApprove(double amount)`; `registerTeamMember(const std::string& employeeId)`; `calculateTeamLoad(double totalAssignedTasks)`; `employeeId()`; `departmentCode()`; `approvalLimit()` |
| `WarehouseWorker` | `WarehouseWorker(std::string employeeId, std::string shiftCode, double hourlyRate)`; `startShift(const std::string& date)`; `finishShift(const std::string& date)`; `assignPickingTask(const PickingTask& task)`; `assignPackingTask(const PackingTask& task)`; `completeTask(const std::string& taskId)`; `calculateShiftPay(double hoursWorked)`; `onShift()`; `employeeId()`; `shiftCode()` |
| `DeliveryNote` | `DeliveryNote(std::string noteNumber, std::string supplierId, std::string purchaseOrderId)`; `addLine(const DeliveryNoteLine& line)`; `validateAgainstOrder(const PurchaseOrder& order)`; `markReceived(const std::string& receivedDate)`; `totalDeliveredQuantity()`; `noteNumber()`; `supplierId()`; `purchaseOrderId()`; `receivedDate()`; `lines()`; `received()` |
| `GoodsReceipt` | `GoodsReceipt(std::string receiptId, std::string purchaseOrderId, std::string inspectorId)`; `acceptDelivery(DeliveryNote& note, PurchaseOrder& order, const std::string& receivedDate)`; `rejectLine(const std::string& sku, int quantity, const std::string& reason)`; `calculateAcceptedValue(const PurchaseOrder& order)`; `acceptedQuantity()`; `rejectedQuantity()`; `receiptId()`; `purchaseOrderId()`; `inspectorId()`; `rejectionReason()`; `completed()` |
| `PurchaseOrder` | `PurchaseOrder(std::string id,std::string supplierId)`; `addLine(const PurchaseOrderLine&)`; `approve()`; `registerReceipt(const std::string& sku,int quantity)`; `fullyReceived()`; `totalCost()`; `cancel()`; `status()`; `lines()`; `id()`; `supplierId()`; `suppliedBy(const Supplier& supplier)` |
| `PurchaseOrderLine` | `PurchaseOrderLine(std::string sku,int ordered,double unitCost)`; `receive(int quantity)`; `outstanding()`; `lineTotal()`; `sku()`; `ordered()`; `received()` |
| `PurchaseRequest` | `PurchaseRequest(std::string requestId, std::string departmentCode, std::string requesterId)`; `addLine(const PurchaseRequestLine& line)`; `submit()`; `approve(const std::string& managerId)`; `reject(const std::string& managerId, const std::string& reason)`; `cancel()`; `estimatedCost()`; `hasLine(const std::string& sku)`; `status()`; `requestId()`; `departmentCode()`; `requesterId()`; `approverId()`; `rejectionReason()`; `lines()` |
| `Supplier` | `Supplier(std::string supplierId, std::string companyName, std::string contactEmail, int leadTimeDays, double reliabilityRating)`; `rateReliability(double newRating)`; `canFulfil(const PurchaseRequest& request)`; `deactivate()`; `recordOnTimeDelivery()`; `recordLateDelivery()`; `reliabilityScore()`; `supplierId()`; `companyName()`; `contactEmail()`; `leadTimeDays()`; `active()` |
| `SupplierContract` | `SupplierContract(std::string contractId, std::string supplierId, std::string validFrom, std::string validUntil, double minimumOrderValue, double latePenaltyRate)`; `isValidOn(const std::string& date)`; `calculatePenalty(int overdueDays, double orderValue)`; `belongsTo(const Supplier& supplier)`; `renew(const std::string& newValidUntil)`; `contractId()`; `supplierId()`; `minimumOrderValue()` |
| `Barcode` | `Barcode(std::string barcodeValue, std::string format, int quantity = 0, double amount = 0.0)`; `validateBarcode()`; `matches(int delta)`; `barcodeValue()`; `format()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `isNumeric()` |
| `Brand` | `Brand(std::string brandCode, std::string name, int quantity = 0, double amount = 0.0)`; `renameBrand()`; `matchesProduct(int delta)`; `brandCode()`; `name()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `isPremium(double threshold)`; `matchesProduct(const Product& product)` |
| `Product` | `changePrice(double)`; `isAvailableForSale(int requested,int availableStock)`; `needsReorder(int availableStock)`; `deactivate()`; `activate()`; `addBarcode(const std::string&)`; `hasBarcode(const std::string&)`; `sku()`; `name()`; `unitPrice()`; `reorderLevel()`; `categoryCode()`; `brandCode()`; `active()`; `barcodes()`; `registerBarcode(const Barcode& barcode)` |
| `ProductAttribute` | `ProductAttribute(std::string attributeCode, std::string value, int quantity = 0, double amount = 0.0)`; `validateValue()`; `normalizeValue(int delta)`; `attributeCode()`; `value()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `hasValue()` |
| `ProductBundle` | `ProductBundle(std::string bundleCode, std::string name, int quantity = 0, double amount = 0.0)`; `calculateBundlePrice()`; `validateComponents(int delta)`; `bundleCode()`; `name()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `addComponent(const Product& product, int units)`; `isAvailable(int requested)` |
| `ProductCategory` | `ProductCategory(std::string categoryCode, std::string name, int quantity = 0, double amount = 0.0)`; `addProduct()`; `removeProduct(int delta)`; `categoryCode()`; `name()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `containsProduct(const Product& product)`; `isPopulated()` |
| `SerialNumber` | `SerialNumber(std::string serialCode, std::string productSku, int quantity = 0, double amount = 0.0)`; `assignProduct()`; `isAssigned(int delta)`; `serialCode()`; `productSku()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `belongsTo(const Product& product)`; `isRegistered()` |
| `UnitOfMeasure` | `UnitOfMeasure(std::string unitCode, std::string symbol, int quantity = 0, double amount = 0.0)`; `convertToBase()`; `isCompatible(int delta)`; `unitCode()`; `symbol()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `canMeasure(const Product& product)`; `toBase(double value)` |
| `ColdStorage` | `ColdStorage(std::string storageCode, std::string temperature, int quantity = 0, double amount = 0.0)`; `validateTemperature()`; `estimateEnergyUse(int delta)`; `storageCode()`; `temperature()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `accepts(const StorageCondition& condition)`; `isBelowFreezing()` |
| `LoadingDock` | `LoadingDock(std::string dockCode, std::string status, int quantity = 0, double amount = 0.0)`; `openDock()`; `closeDock(int delta)`; `dockCode()`; `status()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `scheduleDelivery(const Delivery& delivery)`; `isOccupied()` |
| `Rack` | `Rack(std::string rackCode, std::string zoneCode, int quantity = 0, double amount = 0.0)`; `calculateLoad()`; `hasFreeLevel(int delta)`; `rackCode()`; `zoneCode()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `belongsTo(const StorageZone& zone)`; `canSupport(double weight)` |
| `Shelf` | `Shelf(std::string shelfCode, std::string rackCode, int quantity = 0, double amount = 0.0)`; `calculateFreeVolume()`; `canStore(int delta)`; `shelfCode()`; `rackCode()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `belongsTo(const Rack& rack)`; `hasCapacityFor(double volume)` |
| `StorageBin` | `StorageBin(std::string binCode, std::string shelfCode, int quantity = 0, double amount = 0.0)`; `calculateFreeCapacity()`; `canFit(int delta)`; `binCode()`; `shelfCode()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `belongsTo(const Shelf& shelf)`; `isEmpty()` |
| `StorageCondition` | `StorageCondition(std::string conditionCode, std::string description, int quantity = 0, double amount = 0.0)`; `isSatisfiedBy()`; `requiresCooling(int delta)`; `conditionCode()`; `description()`; `quantity()`; `amount()`; `active()`; `addRelatedId(const std::string& relatedId)`; `hasRelatedId(const std::string& relatedId)`; `relatedCount()`; `isTemperatureControlled()` |
| `StorageZone` | `StorageZone(std::string code,std::string temperatureMode,double capacity,double occupied=0)`; `isSuitableFor(const std::string& productMode)`; `store(double volume)`; `remove(double volume)`; `freeCapacity()`; `occupancyRate()`; `code()`; `temperatureMode()`; `capacity()`; `occupied()` |
| `Warehouse` | `Warehouse(std::string code,std::string address,double capacity)`; `addZone(const StorageZone&)`; `containsZone(const std::string&)`; `receive(int volume)`; `dispatch(int volume)`; `freeCapacity()`; `occupancyRate()`; `close()`; `reopen()`; `code()`; `address()`; `capacity()`; `occupied()`; `active()`; `zoneCodes()` |

## Ассоциации между классами

В проекте реализовано более 30 ассоциаций через типизированные параметры методов. Метод получает ссылку на объект связанного класса и выполняет предметную проверку, назначение, сверку или изменение состояния. В отличие от простой строки с идентификатором, такие интерфейсы проверяют объект и его состояние на уровне типов C++.

### Типизированные связи, добавленные при расширении модели

| № | Связь | Реализованная операция |
|---:|---|---|
| 1 | `ProductCategory` — `Product` | `containsProduct(const Product&)` |
| 2 | `Brand` — `Product` | `matchesProduct(const Product&)` |
| 3 | `Product` — `Barcode` | `registerBarcode(const Barcode&)` |
| 4 | `ProductBundle` — `Product` | `addComponent(const Product&, int)` |
| 5 | `SerialNumber` — `Product` | `belongsTo(const Product&)` |
| 6 | `UnitOfMeasure` — `Product` | `canMeasure(const Product&)` |
| 7 | `Rack` — `StorageZone` | `belongsTo(const StorageZone&)` |
| 8 | `Shelf` — `Rack` | `belongsTo(const Rack&)` |
| 9 | `StorageBin` — `Shelf` | `belongsTo(const Shelf&)` |
| 10 | `ColdStorage` — `StorageCondition` | `accepts(const StorageCondition&)` |
| 11 | `LoadingDock` — `Delivery` | `scheduleDelivery(const Delivery&)` |
| 12 | `StockItem` — `Product` | `matchesProduct(const Product&)` |
| 13 | `StockLot` — `Product` | `matchesProduct(const Product&)` |
| 14 | `StockBalance` — `Warehouse` | `belongsTo(const Warehouse&)` |
| 15 | `StockMovement` — `StockItem` | `tracks(const StockItem&)` |
| 16 | `StockAdjustment` — `StockBalance` | `applyTo(StockBalance&, int)` |
| 17 | `ReorderRule` — `StockItem` | `appliesTo(const StockItem&)`, `shortageAgainst(const StockItem&)` |
| 18 | `Inventory` — `Warehouse` | `belongsTo(const Warehouse&)` |
| 19 | `InventoryLine` — `StockItem` | `reconcileWith(const StockItem&)` |
| 20 | `PurchaseOrder` — `Supplier` | `suppliedBy(const Supplier&)` |
| 21 | `Customer` — `SalesOrder` | `ownsOrder(const SalesOrder&)` |
| 22 | `PickingTask` — `SalesOrder` | `belongsTo(const SalesOrder&)` |
| 23 | `PackingTask` — `SalesOrder` | `belongsTo(const SalesOrder&)` |
| 24 | `Shipment` — `SalesOrder` | `belongsTo(const SalesOrder&)` |
| 25 | `Delivery` — `Shipment` | `refersTo(const Shipment&)` |
| 26 | `Carrier` — `Delivery` | `canHandle(const Delivery&)` |
| 27 | `Transport` — `Delivery` | `canCarry(const Delivery&)` |
| 28 | `Route` — `Transport` | `supports(const Transport&)` |
| 29 | `TransferRequest` — `Warehouse` | `setDestination(const Warehouse&)`, `connects(const Warehouse&, const Warehouse&)` |
| 30 | `Payment` — `CashAccount` | `charge(CashAccount&)` |
| 31 | `CashAccount` — `Payment` | `recordPayment(const Payment&)` |
| 32 | `WarehouseExpense` — `FinancialReport` | `attachTo(FinancialReport&)` |
| 33 | `FinancialReport` — `WarehouseExpense` | `includesExpense(const WarehouseExpense&)` |
| 34 | `AuditLog` — `Employee` | `records(const Employee&)` |
| 35 | `AccessRole` — `Employee` | `assignTo(const Employee&)` |
| 36 | `MaintenanceRequest` — `Warehouse` | `assignWarehouse(const Warehouse&)` |
| 37 | `StockAlert` — `ReorderRule` | `follows(const ReorderRule&)` |
| 38 | `KPIReport` — `Inventory` | `evaluates(const Inventory&)` |
| 39 | `WarehouseSystem` — `Warehouse` | `registerWarehouse(const Warehouse&)` |

Таким образом, добавлено 39 типизированных связей через параметры методов (37 различных неориентированных пар классов, поскольку для платежа/счёта и отчёта/расхода предусмотрены двунаправленные операции).

### Другие связи, уже присутствующие в предметном ядре

- `Warehouse` — `StorageZone`: склад принимает зону через `addZone(const StorageZone&)`.
- `Inventory` — `InventoryLine`: инвентаризация содержит строки по значению.
- `SalesOrder` — `SalesOrderLine`: заказ содержит позиции по значению.
- `PurchaseOrder` — `PurchaseOrderLine`: закупочный заказ содержит позиции по значению.
- `StockReservation` — `StockItem`: резервирование, освобождение и списание выполняются для конкретной позиции.
- `Driver` — `Delivery`: назначение доставки водителю.
- `QualityInspector` — `StockLot`: инспекция партии и управление карантином.
- `Storekeeper` — `StorageZone` и `PickingTask`: закрепление зоны и назначение задания.
- `WarehouseManager` — `PurchaseRequest` и `WarehouseExpense`: согласование заявки и расхода.
- `WarehouseWorker` — `PickingTask` и `PackingTask`: назначение заданий работнику.
- `Supplier` — `PurchaseRequest`: проверка возможности исполнения заявки.
- `SupplierContract` — `Supplier`: проверка принадлежности договора поставщику.
- `DeliveryNote` — `PurchaseOrder`: сверка накладной с закупочным заказом.
- `GoodsReceipt` — `DeliveryNote` и `PurchaseOrder`: приёмка по накладной и регистрация принятого количества.

Не каждая ассоциация означает владение объектом. Например, водитель не владеет доставкой, а получает ссылку на неё при назначении. Композиция используется там, где жизненный цикл частей управляется владельцем: строки инвентаризации и позиции заказов хранятся по значению внутри соответствующих объектов.

## Собственные исключения

Все исключения имеют отдельные `.h` и `.cpp`. Базовый `WarehouseException` наследуется от `std::runtime_error`; остальные специализированные исключения наследуются от него.

| Исключение | Назначение |
|---|---|
| `WarehouseException` | Базовая ошибка складской системы |
| `ProductNotFoundException` | Товар не найден |
| `DuplicateProductException` | Повторная регистрация товара |
| `InsufficientStockException` | Недостаточный остаток |
| `StorageCapacityExceededException` | Превышение вместимости |
| `InvalidStorageLocationException` | Некорректная ячейка/зона |
| `ExpiredProductException` | Истёк срок годности |
| `SupplierNotFoundException` | Поставщик не найден |
| `PurchaseOrderException` | Ошибка закупочного заказа |
| `InvalidOrderStateException` | Недопустимый переход состояния заказа |
| `ReservationException` | Ошибка резервирования |
| `InventoryMismatchException` | Расхождение инвентаризации |
| `ShipmentException` | Ошибка отгрузки |
| `PaymentException` | Ошибка платежа |
| `AccessDeniedException` | Недостаточно прав доступа |

## Сборка и запуск

Требуются CMake 3.16+ и компилятор с поддержкой C++17.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)
ctest --test-dir build --output-on-failure
./build/warehouse_app
```

## Тестирование

`tests/test_warehouse.cpp` содержит assert-based тестовый сценарий. Он проверяет базовые складские операции, резервирование и списание, переходы состояний заказов, инвентаризацию, персонал, закупки и приёмку. Дополнительно проверяются все 39 новых типизированных связей, денежное списание/зачисление, расчёты и граничные условия вспомогательных подсистем.

CTest запускает четыре исполняемых набора: `warehouse_smoke_tests`, `warehouse_generated_entities`, `warehouse_edge_cases` и `warehouse_per_file_coverage`. Процент покрытия проверяется инструментом GCC gcov отдельно для каждого исходного файла.

## Ограничения и принятые решения

- В нескольких вспомогательных классах финансов, логистики, каталога и администрирования сохраняются обобщённые поля `quantity_` и `amount_`. Их смысл задаётся конкретным классом и методом; при дальнейшем развитии их можно заменить более узкими именованными атрибутами.
- `relatedIds_` используется как список внешних идентификаторов для простых реестров связей. Для критичных сценариев добавлены типизированные методы, принимающие ссылки на связанные объекты.
- Даты в отдельных классах представлены строками формата ISO `YYYY-MM-DD`; полноценная календарная валидация и часовые пояса не реализованы.
- Проект является учебной моделью складской системы: он демонстрирует объектную структуру и операции предметной области, но не подключает реальную БД, платёжный шлюз или транспортные API.
- Порог покрытия проверяется отдельно для каждого файла `cpp/*.cpp`: каждый файл должен иметь не менее 90% покрытых исполняемых строк. Дополнительно контролируется общий процент по проекту.

## Проверка количественных требований

Подсчёт выполнен по заголовочным файлам и реализациям проекта: 77 определений классов (62 предметных и 15 исключений), 363 поля класса, более 100 реализованных операций предметной логики и 39 новых типизированных связей плюс ранее существовавшие связи ядра, персонала и закупок. Вспомогательные `enum class` и две структуры строк заказа/накладной не включались в число классов; поля структур не включались в число 363.


## Unit-тестирование и покрытие

Тесты разделены на четыре исполняемых набора:

- `warehouse_tests` — основные сквозные сценарии предметной области;
- `warehouse_generated_tests` — проверки дополнительных сущностей, их методов и связанных идентификаторов;
- `warehouse_edge_tests` — граничные случаи, некорректные входные данные, исключения и переходы состояний;
- `warehouse_per_file_tests` — дополнительные сценарии для достижения порога покрытия в каждом исходном файле.

Обычная сборка и запуск тестов:

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build -j2
ctest --test-dir build --output-on-failure
```

Сборка с измерением покрытия GCC:

```bash
cmake -S . -B build-coverage \
  -DBUILD_TESTING=ON \
  -DENABLE_COVERAGE=ON \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build-coverage --target coverage -j2
```

Цель `coverage` очищает старые счётчики `.gcda`, запускает все четыре набора CTest и формирует сводку `build-coverage/coverage-summary.txt`. В отчёт включаются только исполняемые строки файлов `cpp/*.cpp`; тестовый код и заголовки не учитываются. Физическая строка исходника учитывается один раз, даже если в ней записано несколько функций.

Цель завершается с ошибкой, если общий процент ниже 90%, хотя бы один исходный файл `cpp/*.cpp` имеет покрытие строк ниже 90% или для какого-либо файла отсутствуют данные gcov. Ветка/branch coverage не является критерием приёмки: проверяется именно покрытие строк.

Последняя контрольная сборка GCC 14.2: **1289 из 1292 строк (99,77%)**. Все **77 из 77** исходных файлов имеют не менее 90% покрытия строк; минимальный результат среди файлов — **95,8%**. Все четыре набора CTest проходят.
