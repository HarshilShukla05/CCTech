#include <iostream>
#include <fstream>
#include <string>
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

void menu() {
    cout << "Choose a shape to plot:\n";
    cout << "1. Cuboid\n";
    cout << "2. Cylinder\n";
    cout << "3. Sphere\n";
    cout << "4. Polyline\n";
    cout << "5. Bezier Curve\n";
    cout << "6. Triangle\n";
    cout << "7. Polygon\n";
    cout << "8. 3D line\n";
    cout << "Enter 0 to create a humanoid structure\n";
    cout << "Enter choice: ";
}

void createHumanoid(Structure &humanoid) {
    cout << "Creating a humanoid structure...\n";

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

    char transform;
    cout << "Do you want to apply transformations to the humanoid? (y/n): ";
    cin >> transform;

    if (transform == 'y') {
        int partChoice;
        cout << "Select the part of the humanoid to transform:\n";
        cout << "1. Head\n2. Neck\n3. Body\n4. Left Arm\n5. Right Arm\n6. Left Leg\n7. Right Leg\n";
        cout << "8. Entire Humanoid\n";
        cout << "Enter your choice: ";
        cin >> partChoice;

        if (partChoice >= 1 && partChoice <= 8) {
            int option;
            cout << "1. Translate\n2. Scale\n3. Rotate\n";
            cin >> option;

            if (option == 1) {
                double dx, dy, dz;
                cout << "Enter translation values (dx dy dz): ";
                cin >> dx >> dy >> dz;
                if (partChoice == 8) {
                    humanoid.translate(dx, dy, dz);
                } else {
                    humanoid.translate(dx, dy, dz, partChoice - 1);
                }
            } else if (option == 2) {
                double sx, sy, sz;
                cout << "Enter scaling factors (sx sy sz): ";
                cin >> sx >> sy >> sz;
                if (partChoice == 8) {
                    humanoid.scale(sx, sy, sz);
                } else {
                    humanoid.scale(sx, sy, sz, partChoice - 1);
                }
            } else if (option == 3) {
                double angle;
                char axis;
                cout << "Enter rotation angle and axis (x/y/z): ";
                cin >> angle >> axis;
                if (partChoice == 8) {
                    humanoid.rotate(angle, axis);
                } else {
                    humanoid.rotate(angle, axis, partChoice - 1);
                }
            }

            // Re-plot the humanoid after transformations
            humanoid.plot("data/humanoid_transformed.dat");
        } else {
            cout << "Invalid choice!\n";
        }
    }
}

// void handleStructureTransformations(Structure &structure) {
//     int choice;
//     cout << "1. Transform the entire structure\n";
//     cout << "2. Transform an individual shape\n";
//     cout << "Enter your choice: ";
//     cin >> choice;

//     if (choice == 1) {
//         int option;
//         cout << "1. Translate\n2. Scale\n3. Rotate\n";
//         cin >> option;

//         if (option == 1) {
//             double dx, dy, dz;
//             cout << "Enter translation values (dx dy dz): ";
//             cin >> dx >> dy >> dz;
//             structure.translate(dx, dy, dz);
//         } else if (option == 2) {
//             double sx, sy, sz;
//             cout << "Enter scaling factors (sx sy sz): ";
//             cin >> sx >> sy >> sz;
//             structure.scale(sx, sy, sz);
//         } else if (option == 3) {
//             double angle;
//             char axis;
//             cout << "Enter rotation angle and axis (x/y/z): ";
//             cin >> angle >> axis;
//             structure.rotate(angle, axis);
//         }
//     } else if (choice == 2) {
//         structure.listShapes();
//         int shapeIndex;
//         cout << "Enter the shape index to transform: ";
//         cin >> shapeIndex;
//         --shapeIndex; // Convert to zero-based index

//         if (shapeIndex >= 0 && shapeIndex < static_cast<int>(structure.getShapeCount())) {
//             int option;
//             cout << "1. Translate\n2. Scale\n3. Rotate\n";
//             cin >> option;

//             if (option == 1) {
//                 double dx, dy, dz;
//                 cout << "Enter translation values (dx dy dz): ";
//                 cin >> dx >> dy >> dz;
//                 structure.translate(dx, dy, dz, shapeIndex);
//             } else if (option == 2) {
//                 double sx, sy, sz;
//                 cout << "Enter scaling factors (sx sy sz): ";
//                 cin >> sx >> sy >> sz;
//                 structure.scale(sx, sy, sz, shapeIndex);
//             } else if (option == 3) {
//                 double angle;
//                 char axis;
//                 cout << "Enter rotation angle and axis (x/y/z): ";
//                 cin >> angle >> axis;
//                 structure.rotate(angle, axis, shapeIndex);
//             }
//         } else {
//             cout << "Invalid shape index!\n";
//         }
//     } else {
//         cout << "Invalid choice!\n";
//     }
// }

void handleIndividualShape(int choice) {
    shared_ptr<Shape> shape;

    if (choice == 1) { // Cuboid
        double length, width, height;
        cout << "Enter length, width, height: ";
        cin >> length >> width >> height;
        shape = make_shared<Cuboid>(length, width, height);
    } else if (choice == 2) { // Cylinder
        double radius, height;
        int resolution;
        cout << "Enter radius, height, and resolution: ";
        cin >> radius >> height >> resolution;
        shape = make_shared<Cylinder>(radius, height, resolution);
    } else if (choice == 3) { // Sphere
        double radius;
        int segments;
        cout << "Enter radius and number of segments: ";
        cin >> radius >> segments;
        shape = make_shared<Sphere>(radius, segments);
    } else if (choice == 4) { // Polyline
        int numPoints;
        cout << "Enter the number of points: ";
        cin >> numPoints;
        auto polyline = make_shared<Polyline>();
        for (int i = 0; i < numPoints; ++i) {
            double x, y, z;
            cout << "Enter coordinates for point " << i + 1 << " (x y z): ";
            cin >> x >> y >> z;
            polyline->addPoint(x, y, z);
        }
        shape = polyline;
    } else if (choice == 5) { // Bezier Curve
        int numPoints;
        cout << "Enter the number of control points: ";
        cin >> numPoints;
        auto bezier = make_shared<Bezier>();
        for (int i = 0; i < numPoints; ++i) {
            double x, y, z;
            cout << "Enter coordinates for control point " << i + 1 << " (x y z): ";
            cin >> x >> y >> z;
            bezier->addControlPoint(x, y, z);
        }
        shape = bezier;
    } else if (choice == 6) { // Triangle
        double x1, y1, z1, x2, y2, z2, x3, y3, z3;
        cout << "Enter coordinates of the three vertices (x1 y1 z1 x2 y2 z2 x3 y3 z3): ";
        cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
        shape = make_shared<Triangle>(x1, y1, z1, x2, y2, z2, x3, y3, z3);
    } else if (choice == 7) { // Polygon
        int sides;
        cout << "Enter the number of sides: ";
        cin >> sides;
        auto polygon = make_shared<Polygon>(sides);
        for (int i = 0; i < sides; ++i) {
            double x, y, z;
            cout << "Enter coordinates for vertex " << i + 1 << " (x y z): ";
            cin >> x >> y >> z;
            polygon->addVertex(x, y, z);
        }
        shape = polygon;
    } else if (choice == 8) { // 3D Line
        double x1, y1, z1, x2, y2, z2;
        cout << "Enter coordinates of the two endpoints (x1 y1 z1 x2 y2 z2): ";
        cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
        auto line = make_shared<Line3D>();
        line->setPoints(x1, y1, z1, x2, y2, z2);
        shape = line;
    } else {
        cout << "Invalid choice!\n";
        return;
    }

    // Plot the shape
    string filename = "data/individual_shape.dat";
    shape->plot(filename);

    // Apply transformations
    char transform;
    cout << "Do you want to apply transformations? (y/n): ";
    cin >> transform;

    if (transform == 'y') {
        int option;
        cout << "1. Translate\n2. Scale\n3. Rotate\n";
        cin >> option;

        if (option == 1) {
            double dx, dy, dz;
            cout << "Enter translation values (dx dy dz): ";
            cin >> dx >> dy >> dz;
            shape->translate(dx, dy, dz);
        } else if (option == 2) {
            double sx, sy, sz;
            cout << "Enter scaling factors (sx sy sz): ";
            cin >> sx >> sy >> sz;
            shape->scale(sx, sy, sz);
        } else if (option == 3) {
            double angle;
            char axis;
            cout << "Enter rotation angle and axis (x/y/z): ";
            cin >> angle >> axis;
            shape->rotate(angle, axis);
        }

        // Re-plot the shape after transformations
        shape->plot("data/individual_shape_transformed.dat");
    }
}

int main() {
    int choice;
    menu();
    cin >> choice;

    if (choice == 0) {
        Structure humanoid;
        createHumanoid(humanoid);
        // handleStructureTransformations(humanoid);
        humanoid.plot("data/humanoid_transformed.dat");
    } else if (choice >= 1 && choice <= 8) {
        handleIndividualShape(choice);
    } else {
        cout << "Invalid choice!\n";
    }

    return 0;
}
