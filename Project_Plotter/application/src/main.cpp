#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "mylib.h"

using namespace std;

int main() {
    try {
        int choice;
        cout << "Choose an option:\n1. Plot 3D Line Segment\n2. Plot Cuboid\nEnter choice: ";
        cin >> choice;

        ofstream dataFile("line_data.txt");

        if (choice == 1) {
            // Plot 3D Line
            double x1, y1, z1, x2, y2, z2;
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

            p1.getInterpolatedPoints(dataFile, step, distance, dx, dy, dz, numberOfPointsToInterPolate);
            dataFile << p2.x << " " << p2.y << " " << p2.z << endl;

        } else if (choice == 2) {
            // Plot Cuboid
            double x, y, z, l, b, h;
            cout << "Enter bottom-left corner coordinates (x, y, z): ";
            cin >> x >> y >> z;
            cout << "Enter length, breadth, height: ";
            cin >> l >> b >> h;

            Cuboid cube(x, y, z, l, b, h);
            cube.writeCuboidData(dataFile);
        }

        dataFile.close();
        
        string plotCommand = "gnuplot -e \"splot 'line_data.txt' with lines title 'Shape'; pause -1\"";
        system(plotCommand.c_str());

    } catch (exception &e) {
        cout << "Error: " << e.what() << '\n';
    }
    return 0;
}
