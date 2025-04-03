#include "TriangulatedPlotter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib> // For system()

void TriangulatedPlotter::run() {
    std::string stlFile;
    std::cout << "Enter the name of the STL file (located in the assets folder): ";
    std::cin >> stlFile;

    std::string stlFilePath = "assets/" + stlFile;
    auto vertices = readSTL(stlFilePath);

    if (vertices.empty()) {
        std::cerr << "Error: Failed to read STL file or file is empty.\n";
        return;
    }

    std::string datFilePath = "data/triangulated_output.dat";
    writeToDatFile(vertices, datFilePath);

    plotWithGnuplot(datFilePath);
}

std::vector<std::vector<double>> TriangulatedPlotter::readSTL(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::vector<double>> vertices;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open STL file: " << filename << "\n";
        return vertices;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        iss >> word;

        if (word == "vertex") {
            double x, y, z;
            iss >> x >> y >> z;
            vertices.push_back({x, y, z});
        }
    }

    file.close();
    return vertices;
}

void TriangulatedPlotter::writeToDatFile(const std::vector<std::vector<double>>& vertices, const std::string& filename) {
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        std::cerr << "Error: Could not write to file: " << filename << "\n";
        return;
    }

    // Write vertices and edges for triangulation
    for (size_t i = 0; i < vertices.size(); i += 3) {
        if (i + 2 < vertices.size()) {
            // Write the triangle vertices
            outFile << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n";
            outFile << vertices[i + 1][0] << " " << vertices[i + 1][1] << " " << vertices[i + 1][2] << "\n";
            outFile << vertices[i + 2][0] << " " << vertices[i + 2][1] << " " << vertices[i + 2][2] << "\n";
            outFile << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n"; // Close the triangle
            outFile << "\n"; // Separate triangles
            outFile << "\n"; // Separate triangles
        }
    }

    outFile.close();
    std::cout << "Data written to " << filename << "\n";
}

void TriangulatedPlotter::plotWithGnuplot(const std::string& filename) {
    std::string command = "gnuplot -e \"splot '" + filename + "' with lines; pause -1\"";
    int result = system(command.c_str());

    if (result != 0) {
        std::cerr << "Error: Failed to execute GNUPLOT command.\n";
    }
}
