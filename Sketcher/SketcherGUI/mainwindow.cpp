#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GLWidget.h"
#include "ShapeInputDialog.h"
#include "cuboid.h"

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
            glWidget->update(); // Ensure the OpenGL widget refreshes

           qDebug() << "Painting cuboid with" << cuboid.getEdgeLines().size() << "vertices (line segments)";
  
        }
    }
}
