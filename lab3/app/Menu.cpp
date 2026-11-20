#include "Menu.h"

#include <iostream>
#include <limits>

void Menu::printMainMenu() const {
    std::cout << "\n================================================\n"
              << "          WAREHOUSE MANAGEMENT SYSTEM\n"
              << "================================================\n"
              << " 1. Product catalog (CRUD)\n"
              << " 2. Supplier directory (CRUD)\n"
              << " 3. Employee directory (CRUD)\n"
              << " 4. Customer directory (CRUD)\n"
              << " 5. Stock operations\n"
              << " 6. Sales orders\n"
              << " 7. Procurement\n"
              << " 8. Warehouse information\n"
              << " 9. Run integrated domain demonstration\n"
              << "10. Save data now\n"
              << "\n 0. Save and exit\n"
              << "================================================\n";
}

void Menu::printCrudMenu(const char* title) const {
    std::cout << "\n========== " << title << " ==========\n"
              << "1. Create record\n"
              << "2. Read all records\n"
              << "3. Find record\n"
              << "4. Update record\n"
              << "5. Delete record\n"
              << "0. Back\n";
}

void Menu::printStockMenu() const {
    std::cout << "\n========== STOCK OPERATIONS ==========\n"
              << "1. Show products and stock\n"
              << "2. Receive product\n"
              << "3. Dispatch product\n"
              << "0. Back\n";
}

void Menu::printSalesMenu() const {
    std::cout << "\n========== SALES ORDERS ==========\n"
              << "1. Create sales order\n"
              << "2. Show sales orders\n"
              << "3. Ship sales order\n"
              << "0. Back\n";
}

void Menu::printProcurementMenu() const {
    std::cout << "\n========== PROCUREMENT ==========\n"
              << "1. Show suppliers and contracts\n"
              << "2. Create purchase request\n"
              << "3. Approve purchase request\n"
              << "4. Create purchase order from request\n"
              << "5. Receive supplier delivery\n"
              << "6. Show procurement records\n"
              << "0. Back\n";
}

int Menu::readChoice() const {
    int choice{};
    while (true) {
        std::cout << "Select option: ";
        if (std::cin >> choice) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }

        std::cout << "Invalid input. Enter an integer.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
