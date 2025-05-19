#include "SketchWindow.h"
#include "SketchGLWidget.h"
#include "GLWidget.h"
#include "ExtrudeUtils.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include "FileConverter.h"
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
    QPushButton* stlIntersectBtn = new QPushButton("STL Intersection");

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
    connect(stlIntersectBtn, &QPushButton::clicked, this, &SketchWindow::onStlIntersection);

    QHBoxLayout* buttons = new QHBoxLayout;
    buttons->addWidget(finishShapeBtn);
    buttons->addWidget(unionBtn);
    buttons->addWidget(intersectBtn);
    buttons->addWidget(subtractBtn);
    buttons->addWidget(clearResultBtn);
    buttons->addWidget(extrudeBtn);
    buttons->addWidget(bezierBtn); 
    buttons->addWidget(triIntersectBtn);
    buttons->addWidget(stlIntersectBtn);

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

void SketchWindow::onStlIntersection() {
    loadStlFilesAndIntersect();
}

void SketchWindow::loadStlFilesAndIntersect() {
    stlFileA = QFileDialog::getOpenFileName(this, "Select First STL File", "", "STL Files (*.stl)");
    if (stlFileA.isEmpty()) return;
    stlFileB = QFileDialog::getOpenFileName(this, "Select Second STL File", "", "STL Files (*.stl)");
    if (stlFileB.isEmpty()) return;

    FileConverter converter;
    auto meshA = converter.load(stlFileA.toStdString());
    auto meshB = converter.load(stlFileB.toStdString());

    qDebug() << "STL A vertices:" << meshA.first.size() << "faces:" << meshA.second.size();
    qDebug() << "STL B vertices:" << meshB.first.size() << "faces:" << meshB.second.size();

    if (meshA.first.empty() || meshA.second.empty()) {
        QMessageBox::warning(this, "Error", "Failed to load first STL file or it contains no faces.");
        sketchGLWidget->setStlMeshes({}, {}, {}, {});
        sketchGLWidget->setIntersectingTriangles({});
        sketchGLWidget->update();
        return;
    }
    if (meshB.first.empty() || meshB.second.empty()) {
        QMessageBox::warning(this, "Error", "Failed to load second STL file or it contains no faces.");
        sketchGLWidget->setStlMeshes({}, {}, {}, {});
        sketchGLWidget->setIntersectingTriangles({});
        sketchGLWidget->update();
        return;
    }

    // Warn if mesh is large
    if (meshA.second.size() * meshB.second.size() > 500000) {
        QMessageBox::information(this, "Warning", "Intersection of large STL files may freeze the UI. Please use smaller files for demo.");
    }

    std::vector<std::vector<double>> intersectionTriangles;
    for (const auto& fa : meshA.second) {
        if (fa[0] >= meshA.first.size() || fa[1] >= meshA.first.size() || fa[2] >= meshA.first.size())
            continue;
        Triangle triA;
        triA.v0 = QVector3D(meshA.first[fa[0]][0], meshA.first[fa[0]][1], meshA.first[fa[0]][2]);
        triA.v1 = QVector3D(meshA.first[fa[1]][0], meshA.first[fa[1]][1], meshA.first[fa[1]][2]);
        triA.v2 = QVector3D(meshA.first[fa[2]][0], meshA.first[fa[2]][1], meshA.first[fa[2]][2]);
        for (const auto& fb : meshB.second) {
            if (fb[0] >= meshB.first.size() || fb[1] >= meshB.first.size() || fb[2] >= meshB.first.size())
                continue;
            Triangle triB;
            triB.v0 = QVector3D(meshB.first[fb[0]][0], meshB.first[fb[0]][1], meshB.first[fb[0]][2]);
            triB.v1 = QVector3D(meshB.first[fb[1]][0], meshB.first[fb[1]][1], meshB.first[fb[1]][2]);
            triB.v2 = QVector3D(meshB.first[fb[2]][0], meshB.first[fb[2]][1], meshB.first[fb[2]][2]);
            std::vector<QVector3D> intersections;
            if (triangleIntersectsTriangle(triA, triB, intersections) && !intersections.empty()) {
                for (size_t i = 0; i + 2 < intersections.size(); i += 3) {
                    std::vector<double> triPts;
                    for (int j = 0; j < 3; ++j) {
                        triPts.push_back(intersections[i+j].x());
                        triPts.push_back(intersections[i+j].y());
                        triPts.push_back(intersections[i+j].z());
                    }
                    intersectionTriangles.push_back(triPts);
                }
            }
        }
    }

    sketchGLWidget->setStlMeshes(meshA.first, meshA.second, meshB.first, meshB.second);
    sketchGLWidget->setIntersectingTriangles(intersectionTriangles);
    sketchGLWidget->update();

    if (intersectionTriangles.empty()) {
        QMessageBox::information(this, "STL Intersection", "No intersections found between the selected STL files.");
    }
}