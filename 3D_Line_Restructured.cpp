#include <iostream>
#include <fstream>
#include <cstdlib> // for gnuplot
#include <cmath>


using namespace std;


class Point
{
public:

    double x;
    double y;
    double z;

    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void getInterpolatedPoints(ofstream &dataFile, double step, double distance, double dx, double dy, double dz, int numberOfPointsToInterPolate)
    {
        if (dataFile.is_open())
        {
            // n equally spaced points between (x1, y1, z1) and (x2, y2, z2)
            for (int i = 1; i <= numberOfPointsToInterPolate; ++i)
            {
                double ratio = i * step / distance;
                double x = this->x + ratio * dx;
                double y = this->y + ratio * dy;
                double z = this->z + ratio * dz;
                dataFile << x << " " << y << " " << z << endl;
            }
        }
        else
        {
            cout << "Error opening data file!..." << endl;
            return;
        }
    }
};

int main()
{
    try
    {
        double x1, y1, z1, x2, y2, z2;

        cout << "Hello 3D line segment..." << endl;

        cout << "Enter coordinates for point 1 (x1, y1, z1): ";
        cin >> x1 >> y1 >> z1;
        cout << "Enter coordinates for point 2 (x2, y2, z2): ";
        cin >> x2 >> y2 >> z2;

        Point p1(x1, y1, z1), p2(x2, y2, z2);

        int numberOfPointsToInterPolate;
        cout << "Enter Number of Points to Interpolate :";
        cin >> numberOfPointsToInterPolate;
        
        double dx = p2.x - p1.x;
        double dy = p2.y - p1.y;
        double dz = p2.z - p1.z;
        double distance = sqrt(dx * dx + dy * dy + dz * dz);

        // distance between each consecutive point
        double step = distance / numberOfPointsToInterPolate; // n+1 intervals (n points in between)
        cout << "The equation of the line is: (x - " << x1 << ")/" << dx << " = (y - " << y1 << ")/" << dy << " = (z - " << z1 << ")/" << dz << endl;

        // Adding all points in line_data file 
        ofstream dataFile("line_data.txt");
        dataFile << p1.x << " " << p1.y << " " << p1.z << endl;
        p1.getInterpolatedPoints(dataFile, step, distance, dx, dy, dz, numberOfPointsToInterPolate);
        dataFile << p2.x << " " << p2.y << " " << p2.z << endl;


        string plotCommand = "gnuplot -e \"splot 'line_data.txt' with linespoints title 'Line Segment'; pause -1\"";
        system(plotCommand.c_str());
    }
    catch (exception &e)
    {
        cout << "Error : " << e.what() << '\n';
    }

    
    // remove("line_data.txt");

    return 0;
}
