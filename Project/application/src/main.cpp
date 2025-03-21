#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "mylib.h"

using namespace std;

int main() {
    try {
        double x1, y1, z1, x2, y2, z2;

        cout << "Hello 3D line segment..." << endl;
        cout << "Enter coordinates for point 1 (x1, y1, z1): ";
        cin >> x1 >> y1 >> z1;
        cout << "Enter coordinates for point 2 (x2, y2, z2): ";
        cin >> x2 >> y2 >> z2;

        Point p1(x1, y1, z1), p2(x2, y2, z2);
        
        int numberOfPointsToInterPolate;
        cout << "Enter Number of Points to Interpolate: ";
        cin >> numberOfPointsToInterPolate;

        double dx = p2.x - p1.x;
        double dy = p2.y - p1.y;
        double dz = p2.z - p1.z;
        double distance = sqrt(dx * dx + dy * dy + dz * dz);

        double step = distance / numberOfPointsToInterPolate;
        cout << "The equation of the line is: (x - " << x1 << ")/" << dx << " = (y - " << y1 << ")/" << dy << " = (z - " << z1 << ")/" << dz << endl;

        ofstream dataFile("line_data.txt");
        dataFile << p1.x << " " << p1.y << " " << p1.z << endl;
        p1.getInterpolatedPoints(dataFile, step, distance, dx, dy, dz, numberOfPointsToInterPolate);
        dataFile << p2.x << " " << p2.y << " " << p2.z << endl;

        string plotCommand = "gnuplot -e \"splot 'line_data.txt' with linespoints title 'Line Segment'; pause -1\"";
        system(plotCommand.c_str());
    }
    catch (exception &e) {
        cout << "Error: " << e.what() << '\n';
    }
    return 0;
}
