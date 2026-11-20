# Система управления складом (Warehouse Management System)

Учебный ООП-проект на C++17. Структура и стиль организации исходников подготовлены по образцу предоставленной лабораторной работы «Деканат»: отдельные заголовочные и реализационные файлы, тематическое разбиение подсистем, CMake и тестирование.

## Состав проекта

- 62 предметных класса и 15 собственных классов исключений;
- отдельный `.h` и `.cpp` для каждого класса;
- тематическое разделение по подсистемам;
- CMake-сборка на C++17;
- исполняемый пример и автоматические smoke-тесты.

**Статус реализации.** Основные сценарии предметной области уже реализованы для `Product`, `Warehouse`, `StorageZone`, `StockItem`, `StockLot`, `StockReservation`, `Inventory`, `InventoryLine`, `SalesOrder`, `SalesOrderLine`, `PurchaseOrder` и `PurchaseOrderLine`. В остальных классах пока сохранён исходный каркас из первой версии: их интерфейсы и базовая валидация есть, но часть методов требует дальнейшей предметной реализации. Поэтому количество всех объявленных методов нельзя автоматически считать количеством завершённых уникальных поведений. Этот README явно отделяет реализованное ядро от оставшихся заготовок.

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

Поля определяются назначением конкретной сущности. В старых классах каркаса ещё встречаются универсальные `quantity_`, `amount_`, `active_` и `relatedIds_`; в переработанном ядре они заменены предметными атрибутами (например, `onHand_` и `reserved_` у `StockItem`, `capacity_` и `occupied_` у `Warehouse`, `status_` у заказов). Точные поля каждого класса перечислены в каталоге ниже.

## Каталог классов, полей и основных методов


### Сотрудники и персонал

| Класс | Поля (типы) | Основные методы |
|---|---|---|
| `Employee` | `employeeId_: string`, `fullName_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `validateIdentity()`, `changeRole(int)` |
| `WarehouseWorker` | `employeeId_: string`, `shiftCode_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `startShift()`, `finishShift(int)` |
| `Storekeeper` | `employeeId_: string`, `zoneCode_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `assignZone()`, `countAssignedTasks(int)` |
| `WarehouseManager` | `employeeId_: string`, `departmentCode_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `approveOperation()`, `calculateTeamLoad(int)` |
| `Driver` | `employeeId_: string`, `licenseNumber_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `validateLicense()`, `recordDelivery(int)` |
| `QualityInspector` | `employeeId_: string`, `certificateCode_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `approveQuality()`, `rejectBatch(int)` |

### Номенклатура и товары

| Класс | Поля (типы) | Основные методы |
|---|---|---|
| `Product` | `sku_: string`, `name_: string`, `unitPrice_: double`, `reorderLevel_: int`, `categoryCode_: string`, `brandCode_: string`, `active_: bool`, `barcodes_: vector<string>` | `changePrice(double)`, `isAvailableForSale(int,int)`, `needsReorder(int)`, `addBarcode(string)`, `deactivate()`, `activate()` |
| `ProductCategory` | `categoryCode_: string`, `name_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `addProduct()`, `removeProduct(int)` |
| `Brand` | `brandCode_: string`, `name_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `renameBrand()`, `matchesProduct(int)` |
| `ProductAttribute` | `attributeCode_: string`, `value_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `validateValue()`, `normalizeValue(int)` |
| `UnitOfMeasure` | `unitCode_: string`, `symbol_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `convertToBase()`, `isCompatible(int)` |
| `Barcode` | `barcodeValue_: string`, `format_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `validateBarcode()`, `matches(int)` |
| `SerialNumber` | `serialCode_: string`, `productSku_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `assignProduct()`, `isAssigned(int)` |
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
| `Supplier` | `supplierId_: string`, `companyName_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `rateReliability()`, `canSupply(int)` |
| `PurchaseRequest` | `requestId_: string`, `departmentCode_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `submit()`, `cancel(int)` |
| `PurchaseOrder` | `id_: string`, `supplierId_: string`, `lines_: vector<PurchaseOrderLine>`, `status_: PurchaseOrderStatus` | `addLine(PurchaseOrderLine)`, `approve()`, `registerReceipt(string,int)`, `fullyReceived()`, `totalCost()`, `cancel()` |
| `PurchaseOrderLine` | `sku_: string`, `ordered_: int`, `received_: int`, `unitCost_: double` | `receive(int)`, `outstanding()`, `lineTotal()` |
| `GoodsReceipt` | `receiptId_: string`, `purchaseOrderId_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `acceptDelivery()`, `calculateAcceptedValue(int)` |
| `DeliveryNote` | `noteNumber_: string`, `supplierId_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `validateAgainstOrder()`, `markReceived(int)` |
| `SupplierContract` | `contractId_: string`, `supplierId_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `isValidOn()`, `calculatePenalty(int)` |

### Клиентские заказы и отгрузки

| Класс | Поля (типы) | Основные методы |
|---|---|---|
| `Customer` | `customerId_: string`, `fullName_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `validateContact()`, `updateLoyalty(int)` |
| `SalesOrder` | `orderId_: string`, `customerId_: string`, `lines_: vector<SalesOrderLine>`, `status_: SalesOrderStatus` | `addLine(SalesOrderLine)`, `removeLine(string)`, `calculateTotal()`, `confirm()`, `beginPicking()`, `markShipped()`, `cancel()` |
| `SalesOrderLine` | `sku_: string`, `quantity_: int`, `unitPrice_: double`, `discount_: double` | `lineTotal()`, `changeQuantity(int)`, `applyDiscount(double)` |
| `PickingTask` | `taskId_: string`, `orderId_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `assignPicker()`, `markPicked(int)` |
| `PackingTask` | `taskId_: string`, `orderId_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculatePackageCount()`, `completePacking(int)` |
| `Shipment` | `shipmentId_: string`, `orderId_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `dispatch()`, `markDelivered(int)` |
| `ReturnRequest` | `returnId_: string`, `orderId_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `approveReturn()`, `calculateRefund(int)` |
| `CustomerInvoice` | `invoiceId_: string`, `customerId_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculateDue()`, `markPaid(int)` |

### Логистика

| Класс | Поля (типы) | Основные методы |
|---|---|---|
| `Transport` | `transportId_: string`, `registrationNumber_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `isAvailable()`, `calculateCapacity(int)` |
| `Route` | `routeId_: string`, `destination_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculateDistance()`, `estimateDuration(int)` |
| `Delivery` | `deliveryId_: string`, `shipmentId_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `confirmDelivery()`, `recordFailure(int)` |
| `TransferRequest` | `transferId_: string`, `sourceWarehouse_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `approveTransfer()`, `calculateTransferCost(int)` |
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
| `MaintenanceRequest` | `requestId_: string`, `equipmentCode_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `scheduleMaintenance()`, `closeRequest(int)` |
| `KPIReport` | `reportId_: string`, `period_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `calculateOrderFulfillmentRate()`, `calculateInventoryTurnover(int)` |
| `AccessRole` | `roleCode_: string`, `roleName_: string`, `quantity_: int`, `amount_: double`, `active_: bool`, `relatedIds_: vector<string>` | `grantPermission()`, `revokePermission(int)` |

## Ассоциации между классами

В исходном каркасе многие ассоциации представлены идентификаторами связанных объектов. В переработанном ядре добавлены предметные связи через коллекции конкретных классов, ссылки на объекты и поля внешних ключей. Перечень ниже описывает модель предметной области; не все перечисленные связи уже реализованы типизированными полями — подробности приведены в разделе «Реализованные связи и ограничения».

Основные логические ассоциации:

1. `WarehouseSystem` — `Warehouse` (управляемые склады).
2. `WarehouseSystem` — `Employee` (персонал сети).
3. `Warehouse` — `StorageZone` (зоны склада).
4. `StorageZone` — `Rack` (стеллажи зоны).
5. `Rack` — `Shelf` (полки стеллажа).
6. `Shelf` — `StorageBin` (ячейки хранения).
7. `Warehouse` — `LoadingDock` (погрузочные доки).
8. `ColdStorage` — `StorageCondition` (условия хранения).
9. `Product` — `ProductCategory` (категория товара).
10. `Product` — `Brand` (бренд).
11. `Product` — `ProductAttribute` (характеристики товара).
12. `Product` — `Barcode` (штрихкоды).
13. `Product` — `SerialNumber` (серийные номера).
14. `ProductBundle` — `Product` (состав комплекта).
15. `UnitOfMeasure` — `Product` (единица измерения).
16. `StockItem` — `Product` (учётная позиция).
17. `StockItem` — `Warehouse` (место хранения).
18. `StockLot` — `Product` (партия товара).
19. `StockBalance` — `Warehouse` (остатки склада).
20. `StockReservation` — `SalesOrder` (резерв под заказ).
21. `Inventory` — `Warehouse` (инвентаризируемый склад).
22. `Inventory` — `InventoryLine` (строки пересчёта).
23. `InventoryLine` — `Product` (пересчитываемый товар).
24. `StockMovement` — `Product` (перемещаемый товар).
25. `StockAdjustment` — `StockBalance` (корректируемый остаток).
26. `ReorderRule` — `Product` (правило пополнения товара).
27. `Supplier` — `SupplierContract` (договоры поставщика).
28. `Supplier` — `PurchaseOrder` (заказы поставщику).
29. `PurchaseRequest` — `PurchaseOrder` (заявка, породившая заказ).
30. `PurchaseOrder` — `PurchaseOrderLine` (позиции заказа).
31. `PurchaseOrder` — `GoodsReceipt` (приёмки по заказу).
32. `GoodsReceipt` — `DeliveryNote` (сопроводительная накладная).
33. `Customer` — `SalesOrder` (заказы клиента).
34. `SalesOrder` — `SalesOrderLine` (позиции заказа).
35. `SalesOrder` — `PickingTask` (задание на подбор).
36. `SalesOrder` — `PackingTask` (задание на упаковку).
37. `SalesOrder` — `Shipment` (отгрузка заказа).
38. `SalesOrder` — `CustomerInvoice` (счёт клиенту).
39. `Shipment` — `Delivery` (доставка отправления).
40. `Carrier` — `Delivery` (перевозчик доставки).
41. `Transport` — `Route` (транспорт на маршруте).
42. `TransferRequest` — `Warehouse` (склады-участники перемещения).
43. `Payment` — `CashAccount` (счёт списания/зачисления).
44. `WarehouseExpense` — `FinancialReport` (расход в отчёте).
45. `Employee` — `AccessRole` (роль доступа сотрудника).
46. `Employee` — `AuditLog` (действия сотрудника).
47. `Warehouse` — `MaintenanceRequest` (заявки на обслуживание).
48. `StockAlert` — `ReorderRule` (сигнал о необходимости пополнения).
49. `QualityInspector` — `StockLot` (проверка партии).
50. `Storekeeper` — `StorageZone` (ответственная зона кладовщика).

**Примечание об уровне реализации связей:** в текущем каркасе связи хранятся через идентификаторы (`std::string`), а не через типизированные `shared_ptr`/ссылки на объекты. Перечень выше фиксирует модель предметной области и направления связей; для строгой объектной навигации эти связи можно усилить типизированными ассоциациями при развитии проекта.

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

`tests/test_warehouse.cpp` содержит smoke-тесты:
- создание товара и проверка начального состояния;
- проверка изменения количества;
- добавление и поиск связанного идентификатора;
- операции с остатком;
- проверка обработки некорректных входных данных.

## Границы текущей версии

Это стартовая архитектурная версия большого проекта. Все классы имеют отдельные файлы, конструкторы с проверкой входных данных, поля и два предметно названных метода. Часть поведения пока реализована как общий каркас управления активностью и количеством; методы, требующие полноценной бизнес-модели (например, расчёт маршрута, проведение платежа, сверка накладной и инвентаризация), необходимо развить отдельной логикой и типизированными связями. Поэтому формальные количества классов/полей/методов достигнуты, но проект ещё не следует считать завершённой складской системой.


## Реализованные связи и ограничения

В ядре применены связи с предметными типами, а не универсальный список идентификаторов:

- `Warehouse` принимает `StorageZone` через `addZone(const StorageZone&)` и хранит коды зон; это ассоциация склада с зонами.
- `Inventory` владеет коллекцией `InventoryLine` по значению; это композиция строк инвентаризации.
- `SalesOrder` владеет коллекцией `SalesOrderLine` по значению; это композиция позиций клиентского заказа.
- `PurchaseOrder` владеет коллекцией `PurchaseOrderLine` по значению; это композиция позиций закупки.
- `StockReservation` работает с конкретным `StockItem&` при резервировании, освобождении и списании.
- `Product` хранит коды категории и бренда, а также собственные штрихкоды.

Идентификаторы складов, товаров, ячеек и заказов используются там, где объект является внешним участником операции и его временем жизни управляет другая подсистема. Для учебной модели это намеренная ссылка по ключу, а не владение объектом.

## Проверка

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build -j$(nproc)
ctest --test-dir build --output-on-failure
./build/warehouse_app
```

В smoke-тестах проверяются изменение цены товара, порог пополнения, добавление зоны, контроль вместимости склада, резервирование и списание остатков, переходы состояний клиентского заказа, инвентаризационные расхождения и частичная/полная приёмка закупочного заказа.

## Что ещё необходимо завершить

Оставшиеся классы подсистем персонала, логистики, финансов, администрирования, части каталога и складского оборудования нуждаются в таком же углублении: предметные поля вместо универсальных `quantity_`/`amount_`, чёткие состояния, проверки переходов и ассоциации по типам. Они оставлены в проекте, чтобы сохранить ширину исходной модели (62 предметных класса), но не выдаются за полностью реализованные бизнес-сценарии.
