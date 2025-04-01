#include "structure.h"
#include "cylinder.h"
#include "sphere.h"
#include "cuboid.h"
#include "polyline.h"
#include "bezier.h"
#include "triangle.h"
#include "polygon.h"
#include "line3D.h"
#include <fstream>
#include <iostream>
#include <memory>
#include "plot_utils.h"

using namespace std;

void Structure::addShape(shared_ptr<Shape> shape) {
    shapes.push_back(shape);
}

void Structure::translate(double dx, double dy, double dz, int shapeIndex) {
    if (shapeIndex == -1) { 
        for (auto &shape : shapes) {
            shape->translate(dx, dy, dz);
        }
    } else if (shapeIndex >= 0 && shapeIndex < static_cast<int>(shapes.size())) { 
        shapes[shapeIndex]->translate(dx, dy, dz);
    }
}

void Structure::scale(double sx, double sy, double sz, int shapeIndex) {
    if (shapeIndex == -1) { 
        for (auto &shape : shapes) {
            shape->scale(sx, sy, sz);
        }
    } else if (shapeIndex >= 0 && shapeIndex < static_cast<int>(shapes.size())) { 
        shapes[shapeIndex]->scale(sx, sy, sz);
    }
}

void Structure::rotate(double angle, char axis, int shapeIndex) {
    if (shapeIndex == -1) { 
        for (auto &shape : shapes) {
            shape->rotate(angle, axis);
        }
    } else if (shapeIndex >= 0 && shapeIndex < static_cast<int>(shapes.size())) { 
        shapes[shapeIndex]->rotate(angle, axis);
    }
}

void Structure::saveToFile(const string &filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file for writing.\n";
        return;
    }

    for (const auto &shape : shapes) {
        shape->saveToFile(filename); 
    }

    file.close();
}

void Structure::loadFromFile(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file for reading.\n";
        return;
    }

    shapes.clear();
    string shapeType;
    while (file >> shapeType) {
        if (shapeType == "Cylinder") {
            double r, h;
            int res;
            file >> r >> h >> res;
            shapes.push_back(make_shared<Cylinder>(r, h, res));
        } else if (shapeType == "Sphere") {
            double r;
            int seg;
            file >> r >> seg;
            shapes.push_back(make_shared<Sphere>(r, seg));
        } else if (shapeType == "Cuboid") {
            double length, width, height;
            file >> length >> width >> height;
            shapes.push_back(make_shared<Cuboid>(length, width, height));
        } else if (shapeType == "Polyline") {
            int numPoints;
            file >> numPoints;
            auto polyline = make_shared<Polyline>();
            for (int i = 0; i < numPoints; ++i) {
                double x, y, z;
                file >> x >> y >> z;
                polyline->addPoint(x, y, z);
            }
            shapes.push_back(polyline);
        } else if (shapeType == "Bezier") {
            int numPoints;
            file >> numPoints;
            auto bezier = make_shared<Bezier>();
            for (int i = 0; i < numPoints; ++i) {
                double x, y, z;
                file >> x >> y >> z;
                bezier->addControlPoint(x, y, z);
            }
            shapes.push_back(bezier);
        } else if (shapeType == "Triangle") {
            double x1, y1, z1, x2, y2, z2, x3, y3, z3;
            file >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
            shapes.push_back(make_shared<Triangle>(x1, y1, z1, x2, y2, z2, x3, y3, z3));
        } else if (shapeType == "Polygon") {
            int numVertices;
            file >> numVertices;
            auto polygon = make_shared<Polygon>(numVertices);
            for (int i = 0; i < numVertices; ++i) {
                double x, y, z;
                file >> x >> y >> z;
                polygon->addVertex(x, y, z);
            }
            shapes.push_back(polygon);
        } else if (shapeType == "Line3D") {
            double x1, y1, z1, x2, y2, z2;
            file >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
            auto line = make_shared<Line3D>();
            line->setPoints(x1, y1, z1, x2, y2, z2);
            shapes.push_back(line);
        }
    }

    file.close();
}

void Structure::plot(const std::string &filename) {
    std::ofstream combinedFile(filename);
    if (!combinedFile) {
        std::cerr << "Error: Cannot create combined data file.\n";
        return;
    }

    for (const auto &shape : shapes) {
        shape->saveToFile(filename);
        combinedFile << "\n";
    }
    combinedFile.close();

    PlotUtils::plot(filename, "Structure");
}

void Structure::listShapes() const {
    cout << "Shapes in the structure:\n";
    for (size_t i = 0; i < shapes.size(); ++i) {
        cout << i + 1 << ". " << typeid(*shapes[i]).name() << "\n";
    }
}

size_t Structure::getShapeCount() const {
    return shapes.size();
}

shared_ptr<Shape> Structure::getShape(int index) const {
    if (index >= 0 && index < static_cast<int>(shapes.size())) {
        return shapes[index];
    }
    throw out_of_range("Invalid shape index");
}