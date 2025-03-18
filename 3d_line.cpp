#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

// Function to generate the data file for plotting
void generateDataFile(double x1, double y1, double z1, double x2, double y2, double z2) {
    ofstream file("points3d.dat");
    if (!file) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    // Number of intermediate points (including start and end, total 10)
    int totalPoints = 10;

    // Step sizes for x, y, and z
    double dx = (x2 - x1) / (totalPoints - 1);
    double dy = (y2 - y1) / (totalPoints - 1);
    double dz = (z2 - z1) / (totalPoints - 1);

    // Generate and write points in 3D space
    for (int i = 0; i < totalPoints; i++) {
        double x = x1 + i * dx;
        double y = y1 + i * dy;
        double z = z1 + i * dz;
        file << x << " " << y << " " << z << endl;
    }

    file.close();
}

// Function to plot using Gnuplot
void plotWithGnuplot() {
    FILE* gnuplotPipe = popen("gnuplot -persistent", "w");
    if (!gnuplotPipe) {
        cerr << "Error opening Gnuplot!" << endl;
        return;
    }

    // Gnuplot commands for 3D plotting
    fprintf(gnuplotPipe, "set title '3D Line with 10 Points'\n");
    fprintf(gnuplotPipe, "set xlabel 'X-axis'\n");
    fprintf(gnuplotPipe, "set ylabel 'Y-axis'\n");
    fprintf(gnuplotPipe, "set zlabel 'Z-axis'\n");
    fprintf(gnuplotPipe, "set grid\n");
    fprintf(gnuplotPipe, "splot 'points3d.dat' with linespoints linestyle 1 lw 2 lc rgb 'red' title '3D Line'\n");

    // Close Gnuplot pipe
    pclose(gnuplotPipe);
}

int main() {
    double x1, y1, z1, x2, y2, z2;

    // Taking input from user
    cout << "Enter the first point (x1 y1 z1): ";
    cin >> x1 >> y1 >> z1;
    cout << "Enter the second point (x2 y2 z2): ";
    cin >> x2 >> y2 >> z2;

    // Generate data file and plot
    generateDataFile(x1, y1, z1, x2, y2, z2);
    plotWithGnuplot();

    cout << "3D plot generated using Gnuplot!" << endl;
    return 0;
}
