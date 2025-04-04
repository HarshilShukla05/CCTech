#include "FileConverter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>  // For system()

using namespace std;

void FileConverter::convertObjToStl(const string& objFilename, const string& stlFilename) {
    auto [vertices, faces] = readObj(objFilename);
    if (vertices.empty()) {
        cerr << "Error: Failed to read OBJ file or file is empty.\n";
        return;
    }
    writeStl(vertices, faces, stlFilename);
    cout << "Converted " << objFilename << " to " << stlFilename << "\n";
}

void FileConverter::convertStlToDat(const string& stlFilename, const string& datFilename) {
    auto vertices = readStl(stlFilename);
    if (vertices.empty()) {
        cerr << "Error: Failed to read STL file or file is empty.\n";
        return;
    }

    string datFilePath = "data/" + datFilename;  // Automatically save in the data folder

    // Perform triangulation and write to the DAT file
    ofstream outFile(datFilePath);
    if (!outFile.is_open()) {
        cerr << "Error: Could not write to DAT file: " << datFilePath << "\n";
        return;
    }

    // Triangulation logic: Write vertices and edges for gnuplot
    for (size_t i = 0; i < vertices.size(); i += 3) {
        if (i + 2 < vertices.size()) {
            // Write triangle edges
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

    // Plot the triangulated DAT file using gnuplot
    string gnuplotCommand = "gnuplot -e \"set view equal xyz; splot '" + datFilePath + "' with lines; pause -1\"";
    int result = system(gnuplotCommand.c_str());
    if (result != 0) {
        cerr << "Error: Failed to plot the DAT file using gnuplot.\n";
    }
}

pair<vector<vector<double>>, vector<vector<int>>> 
FileConverter::readObj(const string& filename) {
    ifstream file(filename);
    vector<vector<double>> vertices;
    vector<vector<int>> faces;

    if (!file.is_open()) {
        cerr << "Error: Could not open OBJ file: " << filename << "\n";
        return {vertices, faces};
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string prefix;
        iss >> prefix;

        if (prefix == "v") {  // Vertex line
            double x, y, z;
            iss >> x >> y >> z;
            vertices.push_back({x, y, z});
        } 
        else if (prefix == "f") {  // Face line
            vector<int> face;
            string vertexStr;
            while (iss >> vertexStr) {
                int vertexIndex = stoi(vertexStr) - 1;
                face.push_back(vertexIndex);
            }
            faces.push_back(face);
        }
    }

    file.close();
    return {vertices, faces};
}

void FileConverter::writeStl(const vector<vector<double>>& vertices, 
            const vector<vector<int>>& faces, 
            const string& filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error: Could not write to STL file: " << filename << "\n";
        return;
    }

    outFile << "solid obj_to_stl\n";
    for (const auto& face : faces) {
        if (face.size() < 3) continue;  

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

vector<vector<double>> FileConverter::readStl(const string& filename) {
    ifstream file(filename);
    vector<vector<double>> vertices;

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
            double x, y, z;
            iss >> x >> y >> z;
            vertices.push_back({x, y, z});
        }
    }

    file.close();
    return vertices;
}

void FileConverter::writeDat(const vector<vector<double>>& vertices, 
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
