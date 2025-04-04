#include "ApplicationManager.h"
#include "FileConverter.h"

using namespace std;

void displayMainMenu() {
    cout << "Main Menu:\n";
    cout << "1. Run Shape Plotter\n";
    cout << "2. Convert STL to DAT\n";
    cout << "3. Convert OBJ to STL\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    while (true) {
        displayMainMenu();
        int choice;
        cin >> choice;

        if (choice == 1) {
            ApplicationManager appManager;
            appManager.run();
        } else if (choice == 2) {
            FileConverter converter;
            string stlFile;
            cout << "Enter the name of the STL file (located in the assets folder): ";
            cin >> stlFile;
            string datFile = stlFile.substr(0, stlFile.find_last_of('.')) + ".dat";  // Generate DAT filename
            converter.convertStlToDat("assets/" + stlFile, datFile);
        } else if (choice == 3) {
            FileConverter converter;
            string objFile, stlFile;
            cout << "Enter the name of the OBJ file (located in the assets folder): ";
            cin >> objFile;
            cout << "Enter the name of the STL file to save (in the assets folder): ";
            cin >> stlFile;
            converter.convertObjToStl("assets/" + objFile, "assets/" + stlFile);
        } else if (choice == 0) {
            cout << "Exiting program. Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
