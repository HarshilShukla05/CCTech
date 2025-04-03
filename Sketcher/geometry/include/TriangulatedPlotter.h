#ifndef TRIANGULATED_PLOTTER_H
#define TRIANGULATED_PLOTTER_H

#include <string>
#include <vector>

class TriangulatedPlotter {
public:
    void run(); // Main method to handle STL visualization

private:
    std::vector<std::vector<double>> readSTL(const std::string& filename); // Read STL file
    void writeToDatFile(const std::vector<std::vector<double>>& vertices, const std::string& filename); // Write to .dat file
    void plotWithGnuplot(const std::string& filename); // Plot using GNUPLOT
};

#endif
