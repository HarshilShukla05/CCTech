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

// Cuboid Constructor
Cuboid::Cuboid(double x, double y, double z, double l, double b, double h) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->length = l;
    this->breadth = b;
    this->height = h;
}

void Cuboid::writeCuboidData(ofstream &dataFile) {
    if (!dataFile.is_open()) {
        cout << "Error opening data file!" << endl;
        return;
    }

    // Define the 8 vertices (Bottom and Top Faces)
    double v[8][3] = {
        {x, y, z},                
        {x + length, y, z},        
        {x + length, y + breadth, z},  
        {x, y + breadth, z},       

        {x, y, z + height},       
        {x + length, y, z + height},   
        {x + length, y + breadth, z + height}, 
        {x, y + breadth, z + height}  
    };

    
    int bottomFace[5] = {0, 1, 2, 3,0};  
    int topFace[5] = {4, 5, 6, 7,4};     

    for (int i = 0; i < 4; ++i) {
        dataFile << v[bottomFace[i]][0] << " "
                 << v[bottomFace[i]][1] << " "
                 << v[bottomFace[i]][2] << endl;
    }
   
    dataFile << v[bottomFace[0]][0] << " "
             << v[bottomFace[0]][1] << " "
             << v[bottomFace[0]][2] << endl;
    
    dataFile << endl;  // Blank line to separate faces

    //  top face
    for (int i = 0; i < 4; ++i) {
        dataFile << v[topFace[i]][0] << " "
                 << v[topFace[i]][1] << " "
                 << v[topFace[i]][2] << endl;
    }
    dataFile << endl;
   
    dataFile << v[topFace[0]][0] << " "
             << v[topFace[0]][1] << " "
             << v[topFace[0]][2] << endl;
    
    dataFile << endl;  // Blank line to separate faces

    //Write vertical edges (connecting bottom and top face)
    // for (int i = 0; i < 4; ++i) {
    //     dataFile << v[bottomFace[i]][0] << " "
    //              << v[bottomFace[i]][1] << " "
    //              << v[bottomFace[i]][2] << endl;
    //     dataFile << v[topFace[i]][0] << " "
    //              << v[topFace[i]][1] << " "
    //              << v[topFace[i]][2] << endl;
    //     dataFile << endl;  // Blank line to separate edges
    // }
}


