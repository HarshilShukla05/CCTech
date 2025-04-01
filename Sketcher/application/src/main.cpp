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

// creating shapes
shared_ptr<Shape> createShape(int choice) {
    if (choice == 1) return make_shared<Cuboid>(4, 2, 6);
    if (choice == 2) return make_shared<Cylinder>(1, 5, 20);
    if (choice == 3) return make_shared<Sphere>(2, 20);
    if (choice == 4) {
        auto polyline = make_shared<Polyline>();
        polyline->addPoint(0, 0, 0);
        polyline->addPoint(1, 1, 1);
        return polyline;
    }
    if (choice == 5) {
        auto bezier = make_shared<Bezier>();
        bezier->addControlPoint(0, 0, 0);
        bezier->addControlPoint(1, 1, 1);
        return bezier;
    }
    if (choice == 6) return make_shared<Triangle>(0, 0, 0, 1, 0, 0, 0, 1, 0);
    if (choice == 7) {
        auto polygon = make_shared<Polygon>(4);
        polygon->addVertex(0, 0, 0);
        polygon->addVertex(1, 0, 0);
        polygon->addVertex(1, 1, 0);
        polygon->addVertex(0, 1, 0);
        return polygon;
    }
    if (choice == 8) {
        auto line = make_shared<Line3D>();
        line->setPoints(0, 0, 0, 1, 1, 1);
        return line;
    }
    if (choice == 0) return nullptr; // Reserved for humanoid structure
    return nullptr;
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
        shape->plot("data/cuboid.dat");
    };
    menuOptions[2] = []() {
        auto shape = createShape(2);
        shape->plot("data/cylinder.dat");
    };
    menuOptions[3] = []() {
        auto shape = createShape(3);
        shape->plot("data/sphere.dat");
    };
    menuOptions[4] = []() {
        auto shape = createShape(4);
        shape->plot("data/polyline.dat");
    };
    menuOptions[5] = []() {
        auto shape = createShape(5);
        shape->plot("data/bezier.dat");
    };
    menuOptions[6] = []() {
        auto shape = createShape(6);
        shape->plot("data/triangle.dat");
    };
    menuOptions[7] = []() {
        auto shape = createShape(7);
        shape->plot("data/polygon.dat");
    };
    menuOptions[8] = []() {
        auto shape = createShape(8);
        shape->plot("data/line3D.dat");
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
