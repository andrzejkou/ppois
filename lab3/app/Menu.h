#pragma once

class Menu {
public:
    void printMainMenu() const;
    void printCrudMenu(const char* title) const;
    void printStockMenu() const;
    void printSalesMenu() const;
    void printProcurementMenu() const;
    int readChoice() const;
};
