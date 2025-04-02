#include "plot_utils.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

namespace PlotUtils {
    void plot(const string &filename, const string &title, const string &color) {
        ofstream gnuplotScript("plot_script.gp");
        if (!gnuplotScript) {
            cerr << "Error: Cannot create GNUPLOT script file.\n";
            return;
        }

        gnuplotScript << "set terminal qt size 800,600\n";
        gnuplotScript << "set xlabel 'X'\n";
        gnuplotScript << "set ylabel 'Y'\n";
        gnuplotScript << "set zlabel 'Z'\n";
        gnuplotScript << "set view equal xyz\n";
        gnuplotScript << "splot '" << filename << "' using 1:2:3 with linespoints pointtype 7 linecolor '" << color << "' title '" << title << "'\n";
        gnuplotScript << "pause -1\n";
        gnuplotScript.close();

        system("gnuplot -p plot_script.gp");
    }

    void translate(Matrix &vertices, double dx, double dy, double dz) {
        auto translationMatrix = createTranslationMatrix(dx, dy, dz);
        applyMatrix(vertices, translationMatrix);
    }

    void scale(Matrix &vertices, double sx, double sy, double sz) {
        auto scalingMatrix = createScalingMatrix(sx, sy, sz);
        applyMatrix(vertices, scalingMatrix);
    }

    void rotate(Matrix &vertices, double angle, char axis, double cx, double cy, double cz) {
        // Translate to origin
        translate(vertices, -cx, -cy, -cz);

        // Apply rotation
        auto rotationMatrix = createRotationMatrix(angle, axis);
        applyMatrix(vertices, rotationMatrix);

        // Translate back
        translate(vertices, cx, cy, cz);
    }

    void applyMatrix(Matrix &vertices, const Matrix &matrix) {
        for (auto &vertex : vertices) {
            vector<double> result(3, 0);
            for (int i = 0; i < 3; ++i) {
                result[i] = matrix[i][0] * vertex[0] + matrix[i][1] * vertex[1] + matrix[i][2] * vertex[2] + matrix[i][3];
            }
            vertex = result;
        }
    }

    Matrix createTranslationMatrix(double dx, double dy, double dz) {
        return {
            {1, 0, 0, dx},
            {0, 1, 0, dy},
            {0, 0, 1, dz},
            {0, 0, 0, 1}
        };
    }

    Matrix createScalingMatrix(double sx, double sy, double sz) {
        return {
            {sx, 0,  0,  0},
            {0,  sy, 0,  0},
            {0,  0,  sz, 0},
            {0,  0,  0,  1}
        };
    }

    Matrix createRotationMatrix(double angle, char axis) {
        double radians = angle * M_PI / 180.0;
        if (axis == 'x') {
            return {
                {1, 0,           0,          0},
                {0, cos(radians), -sin(radians), 0},
                {0, sin(radians), cos(radians),  0},
                {0, 0,           0,          1}
            };
        } else if (axis == 'y') {
            return {
                {cos(radians),  0, sin(radians), 0},
                {0,            1, 0,           0},
                {-sin(radians), 0, cos(radians), 0},
                {0,            0, 0,           1}
            };
        } else if (axis == 'z') {
            return {
                {cos(radians), -sin(radians), 0, 0},
                {sin(radians), cos(radians),  0, 0},
                {0,           0,            1, 0},
                {0,           0,            0, 1}
            };
        }
        return {}; 
    }
}
