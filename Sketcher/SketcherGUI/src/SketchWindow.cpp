#include "SketchWindow.h"
#include "SketchGLWidget.h"
#include "GLWidget.h"
#include "ExtrudeUtils.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include "TriangleIntersection.h"

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
    QPushButton* triIntersectBtn = new QPushButton("Triangle-Triangle Intersection");

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
    connect(triIntersectBtn, &QPushButton::clicked, this, &SketchWindow::onTriangleIntersection);

    QHBoxLayout* buttons = new QHBoxLayout;
    buttons->addWidget(finishShapeBtn);
    buttons->addWidget(unionBtn);
    buttons->addWidget(intersectBtn);
    buttons->addWidget(subtractBtn);
    buttons->addWidget(clearResultBtn);
    buttons->addWidget(extrudeBtn);
    buttons->addWidget(bezierBtn); 
    buttons->addWidget(triIntersectBtn);

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

void SketchWindow::onTriangleIntersection() {
    auto polys = sketchGLWidget->getPolygons();  
    if (polys.size() < 2 || polys[0].size() != 3 || polys[1].size() != 3) {
        qDebug() << "Please draw exactly two triangles.";
        return;
    }

    Triangle t1{ QVector3D(polys[0][0]), QVector3D(polys[0][1]), QVector3D(polys[0][2]) };
    Triangle t2{ QVector3D(polys[1][0]), QVector3D(polys[1][1]), QVector3D(polys[1][2]) };

    std::vector<QVector3D> intersects;
    if (triangleIntersectsTriangle(t1, t2, intersects)) {
        qDebug() << "Intersection points:";
        for (const auto& pt : intersects) {
            qDebug() << pt;
        }

        // Optional: visualize intersections
        std::vector<QPointF> proj;
        for (const auto& p : intersects)
            proj.push_back(QPointF(p.x(), p.y()));

        sketchGLWidget->setResultRegion(proj); // New method to draw intersection
    } else {
        qDebug() << "No intersection.";
    }
}