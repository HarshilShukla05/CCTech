#include "SketchGLWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include "polygon_boolean.h"

SketchGLWidget::SketchGLWidget(QWidget* parent)
    : QOpenGLWidget(parent), selectedRegionIndex(-1), resultColor(Qt::transparent) {
    resultRegions.clear();
}

void SketchGLWidget::initializeGL() {
    glClearColor(1, 1, 1, 1);  // White background
}

void SketchGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void SketchGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw original shapes
    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    for (const auto& shape : shapes) {
        for (size_t i = 0; i < shape.size(); ++i) {
            painter.drawLine(shape[i], shape[(i + 1) % shape.size()]);
        }
    }

    // Draw current (unfinished) shape in red
    painter.setPen(QPen(Qt::red, 2));
    for (size_t i = 0; i + 1 < currentPoints.size(); ++i) {
        painter.drawLine(currentPoints[i], currentPoints[i + 1]);
    }

    // Shade the resulting region
    if (!shadedRegion.empty()) {
        QBrush brush(resultColor);
        painter.setBrush(brush);
        QPolygonF polygon;
        for (const auto& pt : shadedRegion) {
            polygon << pt;
        }
        painter.drawPolygon(polygon);
    }
}

bool pointFuzzyEqual(const QPointF& a, const QPointF& b, double eps = 1e-3) {
    return std::abs(a.x() - b.x()) < eps && std::abs(a.y() - b.y()) < eps;
}

void SketchGLWidget::mousePressEvent(QMouseEvent* event) {
    QPointF clicked = event->pos();

    if (!currentPoints.empty() && pointFuzzyEqual(clicked, currentPoints.front())) {
        finishCurrentShape();  // close polygon if clicked near start
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

    const std::vector<QPointF>& poly1 = shapes[0];
    const std::vector<QPointF>& poly2 = shapes[1];

    std::vector<QPointF> result = PolygonBoolean::unionPolygons(poly1, poly2);

    if (!result.empty()) {
        shadedRegion = result;  // Store the union result for shading
        resultColor = QColor(0, 255, 0, 100);  // Green with transparency
        update();  // Trigger a repaint
    } else {
        qDebug() << "[applyUnion] Union failed or returned empty.";
    }
}

void SketchGLWidget::applyIntersection() {
    if (shapes.size() < 2) return;

    const std::vector<QPointF>& poly1 = shapes[0];
    const std::vector<QPointF>& poly2 = shapes[1];

    std::vector<QPointF> result = PolygonBoolean::intersect(poly1, poly2);

    if (!result.empty()) {
        shadedRegion = result;  // Store the intersection result for shading
        resultColor = QColor(255, 0, 0, 100);  // Red with transparency
        update();  // Trigger a repaint
    } else {
        qDebug() << "[applyIntersection] Intersection failed or returned empty.";
    }
}

void SketchGLWidget::applySubtraction(bool isAB) {
    if (shapes.size() < 2) return;

    const std::vector<QPointF>& poly1 = isAB ? shapes[0] : shapes[1];
    const std::vector<QPointF>& poly2 = isAB ? shapes[1] : shapes[0];

    std::vector<QPointF> result = PolygonBoolean::subtractPolygons(poly1, poly2);

    if (!result.empty()) {
        shapes = { result };  // Replace shapes with the result
        update();             // Trigger a repaint
    } else {
        qDebug() << "[applySubtraction] Subtraction failed or returned empty.";
    }
}

void SketchGLWidget::clearResult() {
    shadedRegion.clear();  // Clear the shaded region
    resultColor = Qt::transparent;
    update();  // Trigger a repaint
}

std::vector<std::vector<QPointF>> SketchGLWidget::getPolygons() const {
    return shapes;
}
