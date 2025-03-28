#include "triangle.h"

using namespace std;

Triangle::Triangle(double x1, double y1, double z1,
                   double x2, double y2, double z2,
                   double x3, double y3, double z3) {
    vertices = {{x1, y1, z1}, {x2, y2, z2}, {x3, y3, z3}};
}

void Triangle::translate(double dx, double dy, double dz) {
    for (auto& v : vertices) {
        v[0] += dx;
        v[1] += dy;
        v[2] += dz;
    }
}

void Triangle::scale(double sx, double sy, double sz) {
    for (auto& v : vertices) {
        v[0] *= sx;
        v[1] *= sy;
        v[2] *= sz;
    }
}

void Triangle::scale(double factor) {
    scale(factor, factor, factor); // Delegate to multi-factor scaling
}

void Triangle::rotate(double angle, char axis) {
    // Rotate around the centroid by default
    double cx = 0, cy = 0, cz = 0;
    for (const auto& v : vertices) {
        cx += v[0];
        cy += v[1];
        cz += v[2];
    }
    cx /= vertices.size();
    cy /= vertices.size();
    cz /= vertices.size();

    rotate(angle, axis, cx, cy, cz);
}

void Triangle::rotate(double angle, char axis, double Px, double Py, double Pz) {
    double radians = angle * M_PI / 180.0;

    // Step 1: Translate to origin
    translate(-Px, -Py, -Pz);

    // Step 2: Apply Rotation
    for (auto& v : vertices) {
        double x = v[0], y = v[1], z = v[2];

        if (axis == 'x') {
            v[1] = y * cos(radians) - z * sin(radians);
            v[2] = y * sin(radians) + z * cos(radians);
        } else if (axis == 'y') {
            v[0] = x * cos(radians) + z * sin(radians);
            v[2] = -x * sin(radians) + z * cos(radians);
        } else if (axis == 'z') {
            v[0] = x * cos(radians) - y * sin(radians);
            v[1] = x * sin(radians) + y * cos(radians);
        }
    }

    // Step 3: Translate back
    translate(Px, Py, Pz);
}

void Triangle::plot(const string& filename) const {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    // Write triangle vertices (closing the loop)
    for (const auto& v : vertices) {
        file << v[0] << " " << v[1] << " " << v[2] << endl;
    }
    file << vertices[0][0] << " " << vertices[0][1] << " " << vertices[0][2] << endl;
    file.close();

    // Use GNUPLOT interactively with `qt` terminal
    string command = 
    "gnuplot -p -e \""
    "set terminal wxt; "
    "set xlabel 'X'; "
    "set ylabel 'Y'; "
    "set zlabel 'Z'; "
    "splot '" + filename + "' with linespoints pointtype 7 linecolor 'red'\"";

    int result = system(command.c_str());
    if (result != 0) {
        cerr << "Error: Failed to execute GNUPLOT command." << endl;
    }
}

void Triangle::saveToFile(const string &filename) const {
    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "Error: Cannot open file for writing.\n";
        return;
    }
    file << "Triangle\n";
    for (const auto &v : vertices) {
        file << v[0] << " " << v[1] << " " << v[2] << "\n";
    }
    file.close();
}

void Triangle::loadFromFile(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file for reading.\n";
        return;
    }
    vertices.clear();
    for (int i = 0; i < 3; ++i) {
        double x, y, z;
        file >> x >> y >> z;
        vertices.push_back({x, y, z});
    }
    file.close();
}