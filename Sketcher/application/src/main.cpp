#include "ApplicationManager.h"
#include "TriangulatedPlotter.h"

void displayMainMenu() {
    std::cout << "Main Menu:\n";
    std::cout << "1. Run Shape Plotter\n";
    std::cout << "2. Run STL Visualizer (TriangulatedPlotter)\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    while (true) {
        displayMainMenu();
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            ApplicationManager appManager;
            appManager.run();
        } else if (choice == 2) {
            TriangulatedPlotter triangulatedPlotter;
            triangulatedPlotter.run();
        } else if (choice == 0) {
            std::cout << "Exiting program. Goodbye!\n";
            break;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
