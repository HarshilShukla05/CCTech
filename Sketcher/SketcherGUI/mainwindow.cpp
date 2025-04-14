#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GLWidget.h"
#include "ShapeInputDialog.h"
#include "cuboid.h"
#include "cylinder.h"
#include "sphere.h"
#include "TransformDialogue.h"
#include "plot_utils.h"

#include <QVBoxLayout>

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
            glWidget->update();
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
            glWidget->update();
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
            glWidget->update();
        }
    }
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

void MainWindow::applyTransformations(const std::vector<std::vector<double>> &verts) {
    glWidget->setShapeVertices(verts);
    glWidget->update();
}