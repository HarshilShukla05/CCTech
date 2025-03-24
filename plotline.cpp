#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

// Function to generate the data file for plotting
void generateDataFile(double x1, double y1, double x2, double y2) {
    ofstream file("points.dat");
    if (!file) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    // Calculate slope (m) and y-intercept (c) using y = mx + c
    double m = (y2 - y1) / (x2 - x1); // Slope
    double c = y1 - m * x1;           // Intercept

    // Number of intermediate points (including start and end, total 10)
    int totalPoints = 10;
    
    // Step size for x
    double dx = (x2 - x1) / (totalPoints - 1);

    // Generate and write points using the line equation
    for (int i = 0; i < totalPoints; i++) {
        double x = x1 + i * dx;
        double y = m * x + c;
        file << x << " " << y << endl;
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

    // Gnuplot commands
    fprintf(gnuplotPipe, "set title 'Straight Line with 10 Points'\n");
    fprintf(gnuplotPipe, "set xlabel 'X-axis'\n");
    fprintf(gnuplotPipe, "set ylabel 'Y-axis'\n");
    fprintf(gnuplotPipe, "set grid\n");
    fprintf(gnuplotPipe, "plot 'points.dat' with linespoints linestyle 1 lw 2 lc rgb 'blue' title 'Line'\n");

    // Close Gnuplot pipe
    pclose(gnuplotPipe);
}

int main() {
    double x1, y1, x2, y2;

    // Taking input from user
    cout << "Enter the first point (x1 y1): ";
    cin >> x1 >> y1;
    cout << "Enter the second point (x2 y2): ";
    cin >> x2 >> y2;

    // Ensure x1 != x2 to avoid division by zero
    if (x1 == x2) {
        cerr << "Error: The x-coordinates must be different to calculate the slope!" << endl;
        return 1;
    }

    // Generate data file and plot
    generateDataFile(x1, y1, x2, y2);
    plotWithGnuplot();

    cout << "Plot generated using Gnuplot!" << endl;
    return 0;
}
