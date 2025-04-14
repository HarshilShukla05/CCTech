#include "FileConverter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>  // For system()
#include <chrono> // For timing
#include <iomanip> // For formatting output
#include <array> // For std::array
#include <set>
#include <map>
#include <cmath>

using namespace std;

void FileConverter::convertObjToStl(const string& objFilename, const string& stlFilename) {
    auto start = chrono::high_resolution_clock::now();

    auto objData = readObj(objFilename);
    vector<array<double, 3>> vertices = objData.first;
    vector<array<int, 3>> faces = objData.second;

    if (vertices.empty()) {
        cerr << "Error: Failed to read OBJ file or file is empty.\n";
        return;
    }

    cout << "Number of vertices before conversion: " << vertices.size() + faces.size() * 3 << "\n"; // Assuming duplicates
    cout << "Number of unique vertices after conversion: " << vertices.size() << "\n";
    cout << "Number of triangles: " << faces.size() << "\n";

    writeStl(vertices, faces, stlFilename);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Time taken to read and write: " << elapsed.count() << " seconds\n";

    cout << "Converted " << objFilename << " to " << stlFilename << "\n";
}

void FileConverter::convertStlToDat(const string& stlFilename, const string& datFilename) {
    auto start = chrono::high_resolution_clock::now();

    auto vertices = readStl(stlFilename);
    if (vertices.empty()) {
        cerr << "Error: Failed to read STL file or file is empty.\n";
        return;
    }

    cout << "Number of original vertices: " << vertices.size() << "\n";
    cout << "Number of original triangles: " << vertices.size() / 3 << "\n";

    string datFilePath = "data/" + datFilename;

    ofstream outFile(datFilePath);
    if (!outFile.is_open()) {
        cerr << "Error: Could not write to DAT file: " << datFilePath << "\n";
        return;
    }

    for (size_t i = 0; i < vertices.size(); i += 3) {
        if (i + 2 < vertices.size()) {
            outFile << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n";
            outFile << vertices[i + 1][0] << " " << vertices[i + 1][1] << " " << vertices[i + 1][2] << "\n\n";

            outFile << vertices[i + 1][0] << " " << vertices[i + 1][1] << " " << vertices[i + 1][2] << "\n";
            outFile << vertices[i + 2][0] << " " << vertices[i + 2][1] << " " << vertices[i + 2][2] << "\n\n";

            outFile << vertices[i + 2][0] << " " << vertices[i + 2][1] << " " << vertices[i + 2][2] << "\n";
            outFile << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n\n";
        }
        outFile << "\n\n";
    }

    outFile.close();
    cout << "Converted " << stlFilename << " to " << datFilePath << "\n";

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Time taken to read and write: " << elapsed.count() << " seconds\n";

    string gnuplotCommand = "gnuplot -e \"set view equal xyz; splot '" + datFilePath + "' with lines; pause -1\"";
    int result = system(gnuplotCommand.c_str());
    if (result != 0) {
        cerr << "Error: Failed to plot the DAT file using gnuplot.\n";
    }
}

pair<vector<array<double, 3>>, vector<array<int, 3>>> 
FileConverter::readObj(const string& filename) {
    ifstream file(filename);
    vector<array<double, 3>> uniqueVertices;
    vector<array<int, 3>> faces;
    map<array<double, 3>, int> vertexIndexMap; // Map to store vertex indices
    int currentIndex = 0;

    if (!file.is_open()) {
        cerr << "Error: Could not open OBJ file: " << filename << "\n";
        return {uniqueVertices, faces};
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string prefix;
        iss >> prefix;

        if (prefix == "v") {  // Vertex line
            array<double, 3> vertex;
            iss >> vertex[0] >> vertex[1] >> vertex[2];
            if (vertexIndexMap.find(vertex) == vertexIndexMap.end()) {
                vertexIndexMap[vertex] = currentIndex++;
                uniqueVertices.push_back(vertex);
            }
        } 
        else if (prefix == "f") {  // Face line
            array<int, 3> face;
            for (int i = 0; i < 3; ++i) {
                string vertexStr;
                iss >> vertexStr;
                int vertexIndex = stoi(vertexStr) - 1; // OBJ indices are 1-based
                if (vertexIndex < 0 || vertexIndex >= static_cast<int>(uniqueVertices.size())) {
                    cerr << "Error: Invalid vertex index in face definition.\n";
                    return {uniqueVertices, faces};
                }
                face[i] = vertexIndex;
            }
            faces.push_back(face);
        }
    }

    file.close();
    return {uniqueVertices, faces};
}

void FileConverter::writeStl(const vector<array<double, 3>>& vertices, 
            const vector<array<int, 3>>& faces, 
            const string& filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error: Could not write to STL file: " << filename << "\n";
        return;
    }

    outFile << "solid obj_to_stl\n";
    for (const auto& face : faces) {
        outFile << "  facet normal 0 0 0\n";
        outFile << "    outer loop\n";

        for (int i = 0; i < 3; ++i) { 
            const auto& v = vertices[face[i]];
            outFile << "      vertex " << v[0] << " " << v[1] << " " << v[2] << "\n";
        }

        outFile << "    endloop\n";
        outFile << "  endfacet\n";
    }
    outFile << "endsolid obj_to_stl\n";

    outFile.close();
}

vector<array<double, 3>> FileConverter::readStl(const string& filename) {
    ifstream file(filename);
    vector<array<double, 3>> vertices;

    if (!file.is_open()) {
        cerr << "Error: Could not open STL file: " << filename << "\n";
        return vertices;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string prefix;
        iss >> prefix;

        if (prefix == "vertex") {
            array<double, 3> vertex;
            iss >> vertex[0] >> vertex[1] >> vertex[2];
            vertices.push_back(vertex);
        }
    }

    file.close();
    return vertices;
}

void FileConverter::writeDat(const vector<array<double, 3>>& vertices, 
                             const string& filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error: Could not write to DAT file: " << filename << "\n";
        return;
    }

    for (const auto& vertex : vertices) {
        outFile << vertex[0] << " " << vertex[1] << " " << vertex[2] << "\n";
    }

    outFile.close();
}

bool endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::vector<std::vector<double>> FileConverter::load(const std::string& filePath) {
    if (endsWith(filePath, ".stl")) {
        auto rawVertices = readStl(filePath);
        std::vector<std::vector<double>> flatVertices;
        for (const auto& vertex : rawVertices) {
            flatVertices.push_back({vertex[0], vertex[1], vertex[2]}); // Explicit conversion
        }
        return flatVertices;
    } else if (endsWith(filePath, ".obj")) {
        auto objData = readObj(filePath);
        std::vector<std::vector<double>> flatVertices;
        for (const auto& v : objData.first) {
            flatVertices.push_back({v[0], v[1], v[2]});
        }
        return flatVertices;
    } else {
        std::cerr << "Unsupported file format for load: " << filePath << "\n";
        return {};
    }
}

void FileConverter::save(const std::vector<std::vector<double>>& vertices, const std::string& filePath) {
    if (endsWith(filePath, ".stl")) {
        // Fake triangle connectivity (just write as loose triangles)
        std::vector<std::array<double, 3>> rawVerts;
        for (const auto& v : vertices) {
            if (v.size() == 3)
                rawVerts.push_back({v[0], v[1], v[2]});
        }

        std::vector<std::array<int, 3>> fakeFaces;
        for (size_t i = 0; i + 2 < rawVerts.size(); i += 3) {
            fakeFaces.push_back({static_cast<int>(i), static_cast<int>(i + 1), static_cast<int>(i + 2)});
        }

        writeStl(rawVerts, fakeFaces, filePath);
    } 
    else if (endsWith(filePath, ".obj")) {
        std::ofstream out(filePath);
        if (!out.is_open()) {
            std::cerr << "Failed to open OBJ file for writing: " << filePath << "\n";
            return;
        }

        for (const auto& v : vertices) {
            out << "v " << v[0] << " " << v[1] << " " << v[2] << "\n";
        }

        // Optional: Write dummy faces (only if needed)
        out.close();
    } else {
        std::cerr << "Unsupported file format for save: " << filePath << "\n";
    }
}
