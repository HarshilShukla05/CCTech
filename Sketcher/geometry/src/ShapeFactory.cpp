#include "ShapeFactory.h"

shared_ptr<Shape> ShapeFactory::createShape(int choice) {
    map<int, function<shared_ptr<Shape>()>> shapeCreators = {
        {1, []() {
            double length, width, height;
            cout << "Enter length, width, and height for the cuboid: ";
            cin >> length >> width >> height;
            return make_shared<Cuboid>(length, width, height);
        }},
        {2, []() {
            double radius, height;
            int resolution;
            cout << "Enter radius, height, and resolution for the cylinder: ";
            cin >> radius >> height >> resolution;
            return make_shared<Cylinder>(radius, height, resolution);
        }},
        {3, []() {
            double radius;
            int segments;
            cout << "Enter radius and number of segments for the sphere: ";
            cin >> radius >> segments;
            return make_shared<Sphere>(radius, segments);
        }},
        {4, []() {
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
        }},
        {5, []() {
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
        }},
        {6, []() {
            double x1, y1, z1, x2, y2, z2, x3, y3, z3;
            cout << "Enter coordinates for the three vertices of the triangle (x1 y1 z1 x2 y2 z2 x3 y3 z3): ";
            cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
            return make_shared<Triangle>(x1, y1, z1, x2, y2, z2, x3, y3, z3);
        }},
        {7, []() {
            int numVertices;double radius;
            cout << "Enter the number of vertices and radius for the polygon: ";
            // cout << "Enter the number of vertices for the polygon: ";
            cin >> numVertices;
            auto polygon = make_shared<Polygon>(numVertices, radius);
            // for (int i = 0; i < numVertices; ++i) {
            //     double x, y, z;
            //     cout << "Enter x, y, z for vertex " << i + 1 << ": ";
            //     cin >> x >> y >> z;
            //     polygon->addVertex(x, y, z);
            // }
            return polygon;
        }},
        {8, []() {
            double x1, y1, z1, x2, y2, z2;
            cout << "Enter coordinates for the two points of the 3D line (x1 y1 z1 x2 y2 z2): ";
            cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
            auto line = make_shared<Line3D>();
            line->setPoints(x1, y1, z1, x2, y2, z2);
            return line;
        }}
    };

    if (shapeCreators.find(choice) != shapeCreators.end()) {
        return shapeCreators[choice]();
    } else {
        cout << "Invalid choice!\n";
        return nullptr;
    }
}