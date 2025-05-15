#include "SketchWindow.h"
#include "SketchGLWidget.h"
#include "GLWidget.h"
#include "ExtrudeUtils.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>

SketchWindow::SketchWindow(QWidget* parent)
    : QMainWindow(parent) { // Corrected base class
    sketchGLWidget = new SketchGLWidget(this); // Corrected variable name

    QPushButton* finishShapeBtn = new QPushButton("Finish Shape");
    QPushButton* unionBtn = new QPushButton("Union");
    QPushButton* intersectBtn = new QPushButton("Intersection");
    QPushButton* subtractBtn = new QPushButton("Subtraction");
    QPushButton* clearResultBtn = new QPushButton("Clear Result");
    QPushButton* extrudeBtn = new QPushButton("Extrude");
    QPushButton* bezierBtn = new QPushButton("Bezier Mode");

    connect(finishShapeBtn, &QPushButton::clicked, this, &SketchWindow::onFinishShape);
    connect(unionBtn, &QPushButton::clicked, this, &SketchWindow::onUnion);
    connect(intersectBtn, &QPushButton::clicked, this, &SketchWindow::onIntersection);
    connect(subtractBtn, &QPushButton::clicked, this, &SketchWindow::onSubtraction);
    connect(clearResultBtn, &QPushButton::clicked, sketchGLWidget, &SketchGLWidget::clearResult);
    connect(extrudeBtn, &QPushButton::clicked, this, [this]() {
        bool ok;
        float depth = QInputDialog::getDouble(this, "Extrusion Depth", "Enter depth:", 10.0, 0.1, 1000.0, 1, &ok);
        if (ok) {
            sketchGLWidget->extrudeSelectedRegion(depth);
        }
    });
    connect(bezierBtn, &QPushButton::clicked, sketchGLWidget, &SketchGLWidget::toggleBezierMode);

    QHBoxLayout* buttons = new QHBoxLayout;
    buttons->addWidget(finishShapeBtn);
    buttons->addWidget(unionBtn);
    buttons->addWidget(intersectBtn);
    buttons->addWidget(subtractBtn);
    buttons->addWidget(clearResultBtn);
    buttons->addWidget(extrudeBtn);
    buttons->addWidget(bezierBtn); // <-- Add this line to show the Bezier button

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(sketchGLWidget);
    layout->addLayout(buttons);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    setWindowTitle("2D Sketch Editor");
    resize(800, 600);
}

void SketchWindow::onFinishShape() { sketchGLWidget->finishCurrentShape(); }
void SketchWindow::onUnion() { sketchGLWidget->applyUnion(); }
// void SketchWindow::onIntersection() { sketchGLWidget->applyIntersection(); }

void SketchWindow::onIntersection() {
    if (sketchGLWidget->isBezierModeActive()) {
        sketchGLWidget->findBezierIntersections();
    } else {
        sketchGLWidget->applyIntersection();
    }
}

void SketchWindow::onSubtraction() {
    QStringList options = { "A - B", "B - A" };
    bool ok;
    QString choice = QInputDialog::getItem(this, "Subtraction Operation", 
                                           "Choose operation:", options, 0, false, &ok);
    if (ok && !choice.isEmpty()) {
        if (choice == "A - B") {
            sketchGLWidget->applySubtraction(true);  // A - B
        } else if (choice == "B - A") {
            sketchGLWidget->applySubtraction(false); // B - A
        }
    }
}

