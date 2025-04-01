#include "plot_utils.h"
#include <fstream>
#include <iostream>
#include <cmath>

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

    void translate(vector<vector<double>> &vertices, double dx, double dy, double dz) {
        for (auto &v : vertices) {
            v[0] += dx;
            v[1] += dy;
            v[2] += dz;
        }
    }

    void scale(vector<vector<double>> &vertices, double sx, double sy, double sz) {
        for (auto &v : vertices) {
            v[0] *= sx;
            v[1] *= sy;
            v[2] *= sz;
        }
    }

    void rotate(vector<vector<double>> &vertices, double angle, char axis, double cx, double cy, double cz) {
        double radians = angle * M_PI / 180.0;

        for (auto &v : vertices) {
            double x = v[0] - cx, y = v[1] - cy, z = v[2] - cz;

            if (axis == 'x') {
                v[1] = cy + y * cos(radians) - z * sin(radians);
                v[2] = cz + y * sin(radians) + z * cos(radians);
            } else if (axis == 'y') {
                v[0] = cx + x * cos(radians) + z * sin(radians);
                v[2] = cz - x * sin(radians) + z * cos(radians);
            } else if (axis == 'z') {
                v[0] = cx + x * cos(radians) - y * sin(radians);
                v[1] = cy + x * sin(radians) + y * cos(radians);
            }
        }
    }
}
