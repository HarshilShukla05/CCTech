#ifdef _WIN32
#include <windows.h>   
#endif

#include "GLWidget.h"
#include <QDebug>
#include <GL/gl.h>
#include <GL/glu.h>


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent) {}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black background
    glEnable(GL_DEPTH_TEST);
    qDebug() << "OpenGL initialized";
}

void GLWidget::setShapeVertices(const std::vector<std::vector<double>> &verts) {
    vertices = verts;
    faceIndices.clear(); // Clear face indices for normal shapes
    center = computeCenter(); // Compute center
    update();
}

void GLWidget::setShapeData(const std::vector<std::vector<double>> &verts, const std::vector<std::array<int, 3>> &faces) {
    vertices = verts;
    faceIndices = faces;
    center = computeCenter(); // Compute center
    update();
}

std::vector<std::vector<double>> GLWidget::getShapeVertices() const {
    return vertices;
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = float(w) / float(h ? h : 1);
    float fov = 60.0f;        // degrees
    float nearPlane = 1.0f;
    float farPlane = 1000.0f;

    float top = tan(fov * M_PI / 360.0f) * nearPlane;
    float right = top * aspect;

    glFrustum(-right, right, -top, top, nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
}

QVector3D GLWidget::computeCenter() const {
    QVector3D center(0.0f, 0.0f, 0.0f);
    if (vertices.empty()) return center;

    for (const auto& v : vertices) {
        center += QVector3D(v[0], v[1], v[2]);
    }
    center /= vertices.size();
    return center;
}

QVector3D GLWidget::worldToScreen(const std::vector<double>& worldPt) {
    GLint viewport[4];
    GLdouble modelview[16], projection[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLdouble wx, wy, wz;
    gluProject(worldPt[0], worldPt[1], worldPt[2],
               modelview, projection, viewport,
               &wx, &wy, &wz);

    // Qt uses Y axis going down, OpenGL uses up, so invert Y
    return QVector3D(wx, viewport[3] - wy, wz);
}


void GLWidget::mousePressEvent(QMouseEvent *event) {
    lastMousePosition = event->pos();

    if (bezierShape) {
        lastMousePos = event->pos(); // Needed for drag delta
        makeCurrent(); // Ensure OpenGL context is active

        float minDist = 0.05f; // Selection threshold
        selectedControlPointIndex = -1;

        for (int i = 0; i < bezierShape->getControlPoints().size(); ++i) {
            auto pt = bezierShape->getControlPoints()[i];

            // Convert world to screen
            QVector3D screenPt = worldToScreen(pt);
            float dx = event->pos().x() - screenPt.x();
            float dy = event->pos().y() - screenPt.y();

            if (sqrt(dx * dx + dy * dy) < 10.0) { // pixel threshold
                selectedControlPointIndex = i;
                break;
            }
        }
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (selectedControlPointIndex != -1 && bezierShape) {
        // Calculate world movement based on mouse delta
        QPoint delta = event->pos() - lastMousePos;
        lastMousePos = event->pos();

        float dx = delta.x() * 0.01f;
        float dy = -delta.y() * 0.01f; // Invert Y

        auto pt = bezierShape->getControlPoints()[selectedControlPointIndex];
        pt[0] += dx;
        pt[1] += dy;

        bezierShape->setControlPoint(selectedControlPointIndex, pt);
        bezierShape->setCurveVertices(bezierShape->calculateBezierCurve());
        setShapeVertices(bezierShape->getCurveVertices());
        update();
    } 
    
    else if (event->buttons() & Qt::LeftButton) {
        int dx = event->pos().x() - lastMousePosition.x();
        int dy = event->pos().y() - lastMousePosition.y();

        rotationX += dy;
        rotationY += dx;
    }

    lastMousePosition = event->pos();
    update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    selectedControlPointIndex = -1;
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    zoom += event->angleDelta().y() / 120.0f; // Adjust zoom level
    update();
}

void GLWidget::setBezierShape(Bezier* bezier) {
    bezierShape = bezier;
}


// Paint GL
void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Apply transformations (zoom, translation to center, rotation)
    glTranslatef(0.0f, 0.0f, zoom); // Apply zoom first
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f); // Apply rotation around X-axis
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f); // Apply rotation around Y-axis
    glTranslatef(-center.x(), -center.y(), -center.z()); // Translate to center after rotation

    if (!faceIndices.empty()) {
        // Render using face indices (for OBJ/STL files)
        glBegin(GL_TRIANGLES);
        for (const auto& face : faceIndices) {
            for (int i = 0; i < 3; ++i) {
                const auto& vertex = vertices[face[i]];
                glVertex3d(vertex[0], vertex[1], vertex[2]);
            }
        }
        glEnd();
    } else {
        // Render as lines or points (for shapes without triangles)
        glBegin(GL_LINES);
        for (size_t i = 0; i + 1 < vertices.size(); i += 2) {
            glVertex3d(vertices[i][0], vertices[i][1], vertices[i][2]);
            glVertex3d(vertices[i + 1][0], vertices[i + 1][1], vertices[i + 1][2]);
        }
        glEnd();

        // If vertices are not paired, render remaining points
        glBegin(GL_POINTS);
        if (vertices.size() % 2 != 0) {
            const auto& vertex = vertices.back();
            glVertex3d(vertex[0], vertex[1], vertex[2]);
        }
        glEnd();

        if (bezierShape) {
            glColor3f(1.0f, 0.0f, 0.0f); // red control points
            glPointSize(10.0f);
            glBegin(GL_POINTS);
            for (const auto& pt : bezierShape->getControlPoints()) {
                glVertex3f(pt[0], pt[1], pt[2]);
            }
            glEnd();
        }    
    }
}