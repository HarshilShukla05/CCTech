#include "mylib.h"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

Point::Point(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Point::getInterpolatedPoints(ofstream &dataFile, double step, double distance, double dx, double dy, double dz, int numberOfPointsToInterPolate) {
    if (dataFile.is_open()) {
        for (int i = 1; i <= numberOfPointsToInterPolate; ++i) {
            double ratio = i * step / distance;
            double x = this->x + ratio * dx;
            double y = this->y + ratio * dy;
            double z = this->z + ratio * dz;
            dataFile << x << " " << y << " " << z << endl;
        }
    } else {
        cout << "Error opening data file!..." << endl;
    }
}
