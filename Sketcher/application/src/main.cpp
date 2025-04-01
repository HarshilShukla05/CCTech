#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <functional>
#include "structure.h"
#include "cuboid.h"
#include "cylinder.h"
#include "sphere.h"
#include "polyline.h"
#include "bezier.h"
#include "triangle.h"
#include "polygon.h"
#include "line3D.h"

using namespace std;

shared_ptr<Shape> createShape(int choice) {
    map<int, function<shared_ptr<Shape>()>> shapeCreators;

    shapeCreators[1] = []() {
        double length, width, height;
        cout << "Enter length, width, and height for the cuboid: ";
        cin >> length >> width >> height;
        return make_shared<Cuboid>(length, width, height);
    };
    shapeCreators[2] = []() {
        double radius, height;
        int resolution;
        cout << "Enter radius, height, and resolution for the cylinder: ";
        cin >> radius >> height >> resolution;
        return make_shared<Cylinder>(radius, height, resolution);
    };
    shapeCreators[3] = []() {
        double radius;
        int segments;
        cout << "Enter radius and number of segments for the sphere: ";
        cin >> radius >> segments;
        return make_shared<Sphere>(radius, segments);
    };
    shapeCreators[4] = []() {
        int numPoints;
        cout << "Enter the number of points for the polyline: ";
        cin >> numPoints;
        auto polyline = make_shared<Polyline>();
        for (int i = 0; i < numPoints; ++i) {
            double x, y, z;
            cout << "Enter x, y, z for point " << i + 1 << ": ";
            cin >> x >> y >> z;
            polyline->addPoint(x, y, z);
        }
        return polyline;
    };
    shapeCreators[5] = []() {
        int numControlPoints;
        cout << "Enter the number of control points for the Bezier curve: ";
        cin >> numControlPoints;
        auto bezier = make_shared<Bezier>();
        for (int i = 0; i < numControlPoints; ++i) {
            double x, y, z;
            cout << "Enter x, y, z for control point " << i + 1 << ": ";
            cin >> x >> y >> z;
            bezier->addControlPoint(x, y, z);
        }
        return bezier;
    };
    shapeCreators[6] = []() {
        double x1, y1, z1, x2, y2, z2, x3, y3, z3;
        cout << "Enter coordinates for the three vertices of the triangle (x1 y1 z1 x2 y2 z2 x3 y3 z3): ";
        cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
        return make_shared<Triangle>(x1, y1, z1, x2, y2, z2, x3, y3, z3);
    };
    shapeCreators[7] = []() {
        int numVertices;
        cout << "Enter the number of vertices for the polygon: ";
        cin >> numVertices;
        auto polygon = make_shared<Polygon>(numVertices);
        for (int i = 0; i < numVertices; ++i) {
            double x, y, z;
            cout << "Enter x, y, z for vertex " << i + 1 << ": ";
            cin >> x >> y >> z;
            polygon->addVertex(x, y, z);
        }
        return polygon;
    };
    shapeCreators[8] = []() {
        double x1, y1, z1, x2, y2, z2;
        cout << "Enter coordinates for the two points of the 3D line (x1 y1 z1 x2 y2 z2): ";
        cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
        auto line = make_shared<Line3D>();
        line->setPoints(x1, y1, z1, x2, y2, z2);
        return line;
    };

    if (shapeCreators.find(choice) != shapeCreators.end()) {
        return shapeCreators[choice]();
    } else {
        cout << "Invalid choice!\n";
        return nullptr;
    }
}

//create and plot the humanoid structure
void createAndPlotHumanoid() {
    Structure humanoid;

    // Add humanoid parts
    humanoid.addShape(make_shared<Sphere>(2, 20)); // Head
    humanoid.addShape(make_shared<Cylinder>(1, 0.5, 20)); // Neck
    humanoid.addShape(make_shared<Cuboid>(4, 2, 6)); // Body
    humanoid.addShape(make_shared<Cylinder>(0.5, 4, 20)); // Left Arm
    humanoid.addShape(make_shared<Cylinder>(0.5, 4, 20)); // Right Arm
    humanoid.addShape(make_shared<Cylinder>(0.7, 5, 20)); // Left Leg
    humanoid.addShape(make_shared<Cylinder>(0.7, 5, 20)); // Right Leg

    // Positioning
    humanoid.translate(0, 0, 8, 0); // Head
    humanoid.translate(0, 0, 5.3, 1); // Neck
    humanoid.translate(-2, -1, -0.75, 2); // Body
    humanoid.translate(-2.5, 0, 1.25, 3); // Left Arm
    humanoid.translate(2.5, 0, 1.25, 4); // Right Arm
    humanoid.translate(-1, 0, -5.5, 5); // Left Leg
    humanoid.translate(1, 0, -5.5, 6); // Right Leg

    humanoid.plot("data/humanoid_combined.dat");
    cout << "Humanoid structure plotted! Check the GNUPLOT visualization.\n";
}


void menu() {
    map<int, function<void()>> menuOptions;

    menuOptions[0] = []() {
        createAndPlotHumanoid();
    };
    menuOptions[1] = []() {
        auto shape = createShape(1);
        if (shape) shape->plot("data/cuboid.dat");
    };
    menuOptions[2] = []() {
        auto shape = createShape(2);
        if (shape) shape->plot("data/cylinder.dat");
    };
    menuOptions[3] = []() {
        auto shape = createShape(3);
        if (shape) shape->plot("data/sphere.dat");
    };
    menuOptions[4] = []() {
        auto shape = createShape(4);
        if (shape) shape->plot("data/polyline.dat");
    };
    menuOptions[5] = []() {
        auto shape = createShape(5);
        if (shape) shape->plot("data/bezier.dat");
    };
    menuOptions[6] = []() {
        auto shape = createShape(6);
        if (shape) shape->plot("data/triangle.dat");
    };
    menuOptions[7] = []() {
        auto shape = createShape(7);
        if (shape) shape->plot("data/polygon.dat");
    };
    menuOptions[8] = []() {
        auto shape = createShape(8);
        if (shape) shape->plot("data/line3D.dat");
    };

    cout << "Choose an option:\n";
    cout << "0. Plot Humanoid Structure\n";
    cout << "1. Cuboid\n2. Cylinder\n3. Sphere\n4. Polyline\n5. Bezier Curve\n6. Triangle\n7. Polygon\n8. 3D Line\n";
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;

    if (menuOptions.find(choice) != menuOptions.end()) {
        menuOptions[choice]();
    } else {
        cout << "Invalid choice!\n";
    }
}

int main() {
    menu();
    return 0;
}
