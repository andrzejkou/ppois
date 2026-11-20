# Система управления складом

Формат строки: `Класс | число полей | число методов | связанные классы`. Методы — основные операции предметной логики; конструкторы и getters/setters не учитываются. Ассоциации указаны по типизированным параметрам методов и объектным связям модели.

| Класс | Поля | Методы | Ассоциации (связанные классы) |
|---|---:|---:|---|

**Персонал**
| Employee | 6 | 6 | AccessRole, AuditLog |
| WarehouseWorker | 6 | 6 | PackingTask, PickingTask |
| Storekeeper | 4 | 5 | PickingTask, StorageZone |
| WarehouseManager | 4 | 5 | PurchaseRequest, WarehouseExpense |
| Driver | 6 | 5 | Delivery |
| QualityInspector | 5 | 5 | StockLot |

**Товары**
| Product | 8 | 6 | Barcode, Brand, ProductBundle, ProductCategory, SerialNumber, StockItem, StockLot, UnitOfMeasure |
| ProductCategory | 6 | 2 | Product |
| Brand | 6 | 2 | Product |
| ProductAttribute | 6 | 2 | — |
| UnitOfMeasure | 6 | 2 | Product |
| Barcode | 6 | 2 | Product |
| SerialNumber | 6 | 4 | Product |
| ProductBundle | 6 | 2 | Product |

**Складская инфраструктура**
| Warehouse | 6 | 7 | Inventory, MaintenanceRequest, StockBalance, StorageZone, TransferRequest, WarehouseSystem |
| StorageZone | 4 | 5 | Rack, Storekeeper, Warehouse |
| Rack | 6 | 2 | Shelf, StorageZone |
| Shelf | 6 | 2 | Rack, StorageBin |
| StorageBin | 6 | 2 | Shelf |
| LoadingDock | 6 | 2 | Delivery |
| ColdStorage | 6 | 2 | StorageCondition |
| StorageCondition | 6 | 2 | ColdStorage |

**Учёт запасов**
| StockItem | 6 | 6 | InventoryLine, Product, ReorderRule, StockMovement, StockReservation |
| StockLot | 6 | 5 | Product, QualityInspector |
| StockBalance | 6 | 2 | StockAdjustment, Warehouse |
| StockReservation | 5 | 4 | StockItem |
| Inventory | 4 | 8 | InventoryLine, KPIReport, Warehouse |
| InventoryLine | 4 | 3 | Inventory, StockItem |
| StockMovement | 6 | 2 | StockItem |
| StockAdjustment | 6 | 2 | StockBalance |
| ReorderRule | 6 | 2 | StockAlert, StockItem |

**Закупки**
| Supplier | 8 | 6 | PurchaseOrder, PurchaseRequest, SupplierContract |
| PurchaseRequest | 7 | 7 | Supplier, WarehouseManager |
| PurchaseOrder | 4 | 6 | DeliveryNote, GoodsReceipt, PurchaseOrderLine, Supplier |
| PurchaseOrderLine | 4 | 3 | PurchaseOrder |
| GoodsReceipt | 7 | 3 | DeliveryNote, PurchaseOrder |
| DeliveryNote | 6 | 4 | GoodsReceipt, PurchaseOrder |
| SupplierContract | 6 | 4 | Supplier |

**Заказы и отгрузки**
| Customer | 6 | 2 | SalesOrder |
| SalesOrder | 4 | 7 | Customer, PackingTask, PickingTask, SalesOrderLine, Shipment |
| SalesOrderLine | 4 | 3 | SalesOrder |
| CustomerInvoice | 7 | 4 | — |
| PickingTask | 7 | 4 | SalesOrder, Storekeeper, WarehouseWorker |
| PackingTask | 6 | 2 | SalesOrder, WarehouseWorker |
| ReturnRequest | 6 | 2 | — |
| Shipment | 6 | 2 | Delivery, SalesOrder |

**Логистика**
| Carrier | 6 | 2 | Delivery |
| Delivery | 6 | 2 | Carrier, Driver, LoadingDock, Shipment, Transport |
| Route | 6 | 2 | Transport |
| TransferRequest | 7 | 2 | Warehouse |
| Transport | 6 | 2 | Delivery, Route |

**Финансы**
| CashAccount | 6 | 2 | Payment |
| CostCalculation | 6 | 2 | — |
| FinancialReport | 6 | 2 | WarehouseExpense |
| Payment | 6 | 2 | CashAccount |
| WarehouseExpense | 6 | 2 | FinancialReport, WarehouseManager |

**Администрирование**
| AccessRole | 6 | 2 | Employee |
| AuditLog | 6 | 2 | Employee |
| KPIReport | 6 | 2 | Inventory |
| MaintenanceRequest | 7 | 2 | Warehouse |
| StockAlert | 6 | 2 | ReorderRule |
| WarehouseSystem | 6 | 2 | Warehouse |

### Исключения (15)

- WarehouseException
- ProductNotFoundException
- DuplicateProductException
- InsufficientStockException
- StorageCapacityExceededException
- InvalidStorageLocationException
- ExpiredProductException
- SupplierNotFoundException
- PurchaseOrderException
- InvalidOrderStateException
- ReservationException
- InventoryMismatchException
- ShipmentException
- PaymentException
- AccessDeniedException

### Итоговая статистика

| Показатель | Значение |
|---|---:|
| Предметные классы | 62 |
| Поля | 362 |
| Поведения (основные методы предметной логики) | 203 |
| Уникальные связи между классами | 55 |
| Собственные исключения | 15 |

### Сборка и тестирование

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build -j2
ctest --test-dir build --output-on-failure
```

Проверка покрытия строк отдельно для каждого файла `cpp/*.cpp`:

```bash
cmake -S . -B build-coverage -DBUILD_TESTING=ON -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-coverage --target coverage -j2
```

Цель `coverage` проверяет, что каждый исходный `.cpp` имеет не менее 90% покрытия строк. Результаты находятся в `build-coverage/coverage-summary.txt`.
