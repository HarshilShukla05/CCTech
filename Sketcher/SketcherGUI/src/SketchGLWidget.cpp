#include "SketchGLWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QOpenGLFunctions> // <-- This is correct; do not use <OpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>
#include "polygon_boolean.h"
#include "ExtrudeUtils.h" // Include extrusion utilities

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

SketchGLWidget::SketchGLWidget(QWidget* parent)
    : QOpenGLWidget(parent), selectedRegionIndex(-1), resultColor(Qt::transparent) {
    resultRegions.clear();
}

void SketchGLWidget::initializeGL() {
    QOpenGLContext::currentContext()->functions()->initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 1);  // White background
}

void SketchGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void SketchGLWidget::paintGL() {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    for (const auto& shape : shapes) {
        for (size_t i = 0; i < shape.size(); ++i) {
            painter.drawLine(shape[i], shape[(i + 1) % shape.size()]);
        }
    }

    painter.setPen(QPen(Qt::red, 2));
    for (size_t i = 0; i + 1 < currentPoints.size(); ++i) {
        painter.drawLine(currentPoints[i], currentPoints[i + 1]);
    }

    for (int i = 0; i < resultRegions.size(); ++i) {
        QBrush brush(i == selectedRegionIndex ? QColor(255, 255, 0, 120) : QColor(100, 100, 255, 100));
        painter.setBrush(brush);
        QPolygonF poly;
        for (const auto& pt : resultRegions[i])
            poly << pt;
        painter.drawPolygon(poly);
    }
    painter.end();

    // Draw extruded geometry if available
    if (!extrudedVertices.empty() && !extrudedIndices.empty()) {
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, double(width()) / height(), 1.0, 1000.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0, 0, -300.0f); // Camera distance
        glRotatef(30, 1, 0, 0);      // Tilt
        glRotatef(45, 0, 1, 0);      // Rotate

        glColor3f(0.2f, 0.7f, 0.9f); // Cyan
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < extrudedIndices.size(); i += 3) {
            for (int j = 0; j < 3; ++j) {
                const auto& v = extrudedVertices[extrudedIndices[i + j]];
                glVertex3f(v.x(), v.y(), v.z());
            }
        }
        glEnd();

        glDisable(GL_DEPTH_TEST);
    }
}

std::vector<QVector3D> extrudedVertices; // Declare extruded vertices
std::vector<unsigned int> extrudedIndices; // Declare extruded indices

void SketchGLWidget::drawExtrudedGeometry() {
    if (extrudedVertices.empty() || extrudedIndices.empty()) return;

    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, extrudedVertices.data());
    f->glDrawElements(GL_TRIANGLES, extrudedIndices.size(), GL_UNSIGNED_INT, extrudedIndices.data());
    f->glDisableVertexAttribArray(0);
}

bool pointFuzzyEqual(const QPointF& a, const QPointF& b, double eps) {
    return std::abs(a.x() - b.x()) < eps && std::abs(a.y() - b.y()) < eps;
}


void SketchGLWidget::mousePressEvent(QMouseEvent* event) {
    QPointF clicked = event->pos();
    selectedRegionIndex = -1;
    for (int i = 0; i < resultRegions.size(); ++i) {
        if (pointInPolygon(clicked, resultRegions[i])) {
            selectedRegionIndex = i;
            update();
            return;
        }
    }

    if (!currentPoints.empty() && pointFuzzyEqual(clicked, currentPoints.front())) {
        finishCurrentShape();
    } else {
        currentPoints.push_back(clicked);
    }
    update();
}

void SketchGLWidget::finishCurrentShape() {
    if (currentPoints.size() < 3) return;

    shapes.push_back(currentPoints);
    qDebug() << "Added shape with" << currentPoints.size()-1 << "points.";
    currentPoints.clear();
    update();
}

void SketchGLWidget::applyUnion() {
    if (shapes.size() < 2) return;
    const auto& poly1 = shapes[0];
    const auto& poly2 = shapes[1];
    auto result = PolygonBoolean::unionPolygons(poly1, poly2);
    if (!result.empty()) {
        resultRegions.clear();
        resultRegions.push_back(result);
        selectedRegionIndex = 0;
        update();
    }
}

void SketchGLWidget::applyIntersection() {
    if (shapes.size() < 2) return;
    const auto& poly1 = shapes[0];
    const auto& poly2 = shapes[1];
    auto result = PolygonBoolean::intersect(poly1, poly2);
    if (!result.empty()) {
        resultRegions.clear();
        resultRegions.push_back(result);
        selectedRegionIndex = 0;
        update();
    }
}

void SketchGLWidget::applySubtraction(bool isAB) {
    if (shapes.size() < 2) return;
    const auto& poly1 = isAB ? shapes[0] : shapes[1];
    const auto& poly2 = isAB ? shapes[1] : shapes[0];
    auto result = PolygonBoolean::subtractPolygons(poly1, poly2);
    if (!result.empty()) {
        shapes = { result };
        resultRegions.clear();
        selectedRegionIndex = -1;
        update();
    }
}

void SketchGLWidget::clearResult() {
    resultRegions.clear();
    selectedRegionIndex = -1;
    resultColor = Qt::transparent;
    update();
}

std::vector<std::vector<QPointF>> SketchGLWidget::getPolygons() const {
    return shapes;
}

std::vector<QPointF> SketchGLWidget::getSelectedPolygon() const {
    if (selectedRegionIndex >= 0 && selectedRegionIndex < resultRegions.size())
        return resultRegions[selectedRegionIndex];
    return {};
}

void SketchGLWidget::setExtrudedData(const std::vector<QVector3D>& verts, const std::vector<unsigned int>& indices) {
    extrudedVertices = verts;
    extrudedIndices = indices;
}

void SketchGLWidget::extrudeSelectedRegion(float depth) {
    if (resultRegions.empty()) {
        qDebug() << "No regions available for extrusion.";
        return;
    }

    std::vector<QVector3D> allVertices;
    std::vector<unsigned int> allIndices;
    unsigned int vertexOffset = 0;

    for (const auto& region : resultRegions) {
        if (region.size() < 3) continue;

        QPolygonF polygon;
        for (const auto& pt : region)
            polygon << pt;

        std::vector<QVector3D> regionVerts;
        std::vector<unsigned int> regionIndices;

        ExtrudeUtils::generateExtrusion(polygon, depth, regionVerts, regionIndices);

        // Offset indices to match global vertex list
        for (auto& idx : regionIndices)
            idx += vertexOffset;

        allVertices.insert(allVertices.end(), regionVerts.begin(), regionVerts.end());
        allIndices.insert(allIndices.end(), regionIndices.begin(), regionIndices.end());

        vertexOffset += regionVerts.size();
    }

    setExtrudedData(allVertices, allIndices); 

    qDebug() << "Extruded" << resultRegions.size() << "regions into"
             << allVertices.size() << "vertices and"
             << allIndices.size() << "indices.";

    update(); // triggers paintGL()
}


