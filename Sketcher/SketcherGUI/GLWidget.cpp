#include "GLWidget.h"
#include <QDebug>

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
    float near = 1.0f;
    float far = 1000.0f;

    float top = tan(fov * M_PI / 360.0f) * near;
    float right = top * aspect;

    glFrustum(-right, right, -top, top, near, far);

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

void GLWidget::mousePressEvent(QMouseEvent *event) {
    lastMousePosition = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->pos().x() - lastMousePosition.x();
    int dy = event->pos().y() - lastMousePosition.y();

    if (event->buttons() & Qt::LeftButton) {
        rotationX += dy;
        rotationY += dx;
    }

    lastMousePosition = event->pos();
    update();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    zoom += event->angleDelta().y() / 120.0f; // Adjust zoom level
    update();
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
    }
}