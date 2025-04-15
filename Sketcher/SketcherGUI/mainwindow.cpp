#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GLWidget.h"
#include "ShapeInputDialog.h"
#include "cuboid.h"
#include "cylinder.h"
#include "sphere.h"
#include "bezier.h"
#include "polygon.h"
#include "line3D.h"
#include "polyline.h"
#include "TransformDialogue.h"
#include "plot_utils.h"
#include "FileConverter.h"


#include <QVBoxLayout>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setCentralWidget(ui->centralwidget);
    glWidget = new GLWidget(this);
    ui->openGLContainer->setLayout(new QVBoxLayout);
    ui->openGLContainer->layout()->addWidget(glWidget);

    connect(ui->shapeComboBox, &QComboBox::currentTextChanged,
        this, &MainWindow::onShapeSelected);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onShapeSelected(const QString &shape) {
    qDebug() << "Selected shape:" << shape;

    if (shape == "Cuboid") {
        ShapeInputDialog dialog(shape, this);
        if (dialog.exec() == QDialog::Accepted) {
            QMap<QString, double> values = dialog.getInputs();
            double length = values["Length"];
            double width = values["Width"];
            double height = values["Height"];

            Cuboid cuboid(length, width, height);
            glWidget->setShapeVertices(cuboid.getEdgeLines());
        }
    } else if (shape == "Cylinder") {
        ShapeInputDialog dialog(shape, this);
        if (dialog.exec() == QDialog::Accepted) {
            QMap<QString, double> values = dialog.getInputs();
            double radius = values["Radius"];
            double height = values["Height"];
            int resolution = static_cast<int>(values["Resolution"]);

            Cylinder cylinder(radius, height, resolution);
            glWidget->setShapeVertices(cylinder.getEdgeLines());
        }
    }
    else if (shape == "Sphere") {
        ShapeInputDialog dialog(shape, this);
        if (dialog.exec() == QDialog::Accepted) {
            QMap<QString, double> values = dialog.getInputs();
            double radius = values["Radius"];
            int segments = static_cast<int>(values["Segments"]);

            Sphere sphere(radius, segments);
            glWidget->setShapeVertices(sphere.getEdgeLines());
        }
    }
    else if (shape == "Bezier") {
        ShapeInputDialog dialog(shape, this);
        if (dialog.exec() == QDialog::Accepted) {
            QList<QList<double>> controlPoints = dialog.getBezierControlPoints();
            Bezier bezier;
            for (const QList<double> &pt : controlPoints) {
                if (pt.size() == 3)
                    bezier.addControlPoint(pt[0], pt[1], pt[2]);
            }
    
            std::vector<std::vector<double>> curve = bezier.calculateBezierCurve(100);
            glWidget->setShapeVertices(curve);
        }
    } 
    else if (shape == "Polygon") {
        ShapeInputDialog dialog(shape, this);
        if (dialog.exec() == QDialog::Accepted) {
            int numSides = dialog.getInputs()["Number of Sides"];  // Get number of sides
            double radius = dialog.getInputs()["Radius"];  // Get radius for the polygon
    
            Polygon polygon(numSides, radius); 
            glWidget->setShapeVertices(polygon.getEdgeLines());  
        }
    }
    else if (shape == "3DLine") {
        ShapeInputDialog dialog(shape, this);
        if (dialog.exec() == QDialog::Accepted) {
            QMap<QString, double> values = dialog.getInputs();
            double x1 = values["X1"];
            double y1 = values["Y1"];
            double z1 = values["Z1"];
            double x2 = values["X2"];
            double y2 = values["Y2"];
            double z2 = values["Z2"];

            Line3D line;
            line.setPoints(x1, y1, z1, x2, y2, z2);
            glWidget->setShapeVertices(line.getEdgeLines());
        }
    }
    else if (shape == "Polyline") {
        ShapeInputDialog dialog(shape, this);
        if (dialog.exec() == QDialog::Accepted) {
            QVector<QVector<double>> points = dialog.getPolylinePoints();

            Polyline polyline;
            for (const auto &point : points) {
                polyline.addPoint(point[0], point[1], point[2]);
            }

            glWidget->setShapeVertices(polyline.getEdgeLines()); // Corrected type
        }
    }
    glWidget->update();
}

void MainWindow::on_transformButton_clicked() {
    TransformDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QMap<QString, double> values = dialog.getValues();
        QStringList selected = dialog.getSelectedTransforms();

        std::vector<std::vector<double>> verts = glWidget->getShapeVertices();

        if (selected.contains("Translate")) {
            PlotUtils::translate(verts, values["dx"], values["dy"], values["dz"]);
        }

        if (selected.contains("Rotate")) {
            char axis = 'x';
            int axisVal = static_cast<int>(values["axis"]);
            if (axisVal == 1) axis = 'y';
            else if (axisVal == 2) axis = 'z';
            PlotUtils::rotate(verts, values["angle"], axis);
        }

        if (selected.contains("Scale")) {
            PlotUtils::scale(verts, values["sx"], values["sy"], values["sz"]);
        }

        glWidget->setShapeVertices(verts);
        glWidget->update();
    }
}

void MainWindow::on_loadFileButton_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", "", "3D Files (*.obj *.stl)");
    if (!filePath.isEmpty()) {
        FileConverter converter;
        auto result = converter.load(filePath.toStdString());
        const auto& vertices = result.first;
        const auto& faces = result.second;

        if (!vertices.empty()) {
            glWidget->setShapeData(vertices, faces);
        } else {
            qDebug() << "Failed to load file or no geometry data.";
        }
    }
}

void MainWindow::on_saveFileButton_clicked() {
    QString filePath = QFileDialog::getSaveFileName(this, "Save File", "", "3D Files (*.obj *.stl)");
    if (!filePath.isEmpty()) {
        FileConverter converter;
        std::vector<std::vector<double>> vertices = glWidget->getShapeVertices();  // Get current shape
        converter.save(vertices, filePath.toStdString());
    }
}

void MainWindow::applyTransformations(const std::vector<std::vector<double>> &verts) {
    glWidget->setShapeVertices(verts);
    glWidget->update();
}