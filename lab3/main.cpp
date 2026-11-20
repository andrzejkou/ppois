#include "app/Application.h"

#include <exception>
#include <iostream>

int main() {
    try {
        Application application;
        application.run();
    } catch (const std::exception& error) {
        std::cerr << "Fatal application error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
