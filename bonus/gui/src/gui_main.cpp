#include "simple_gui.hpp"
#include <iostream>

int main() {
    try {
        SimpleGUI::run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
