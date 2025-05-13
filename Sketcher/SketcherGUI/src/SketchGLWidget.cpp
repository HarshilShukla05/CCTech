#include "SketchGLWidget.h"
#include <QPainter>
#include <QDebug>
#include "polygon_boolean.h"

SketchGLWidget::SketchGLWidget(QWidget* parent)
    : QOpenGLWidget(parent) {}

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

    QPen pen(Qt::black, 2);
    painter.setPen(pen);

    for (const auto& shape : shapes) {
        for (size_t i = 0; i < shape.size(); ++i) {
            painter.drawLine(shape[i], shape[(i + 1) % shape.size()]);
        }
    }

    // Current (unfinished) shape in red
    painter.setPen(QPen(Qt::red, 2));
    for (size_t i = 0; i + 1 < currentPoints.size(); ++i) {
        painter.drawLine(currentPoints[i], currentPoints[i + 1]);
    }
}

// void SketchGLWidget::paintGL() {
//     glClear(GL_COLOR_BUFFER_BIT);
//     QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing);

//     // Draw filled polygons
//     painter.setBrush(QBrush(Qt::lightGray)); // Optional fill color
//     painter.setPen(QPen(Qt::black, 2));
//     for (const auto& shape : shapes) {
//         if (shape.size() >= 3) {
//             QPolygonF polygon;
//             for (const auto& pt : shape) {
//                 polygon << pt;
//             }
//             painter.drawPolygon(polygon);
//         }
//     }

//     // Draw current shape in red
//     painter.setPen(QPen(Qt::red, 2));
//     QPolygonF currentPolygon;
//     for (const auto& pt : currentPoints) {
//         currentPolygon << pt;
//     }
//     painter.drawPolyline(currentPolygon);
// }

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

    // if (!pointFuzzyEqual(currentPoints.front(), currentPoints.back())) {
    //     currentPoints.push_back(currentPoints.front());
    // }

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
        shapes.erase(shapes.begin());  // remove shape[0]
        shapes[0] = result;            // replace shape[1] with result
        update();                      // trigger a repaint
    } else {
        qDebug() << "[applyUnion] Union failed or returned empty.";
    }
}


void SketchGLWidget::applyIntersection() {
    if (shapes.size() < 2) return;

    auto result = PolygonBoolean::intersect(shapes[0], shapes[1]);
    
    // if (!result.empty() && result.front() != result.back()) {
    //     result.push_back(result.front());
    // }

    if (!result.empty()) {
        shapes = { result };  
        update();
    } else {
        qDebug() << "No intersection found.";
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

std::vector<std::vector<QPointF>> SketchGLWidget::getPolygons() const {
    return shapes;
}
