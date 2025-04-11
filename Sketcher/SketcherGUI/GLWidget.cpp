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
    update();
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
    lastMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx = static_cast<int>(event->position().x() - lastMousePos.x());
    int dy = static_cast<int>(event->position().y() - lastMousePos.y());

    if (event->buttons() & Qt::LeftButton) {
        rotationX += dy;
        rotationY += dx;
        update();
    }

    lastMousePos = event->position().toPoint();
    // event->accept();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0)
        zoom -= 5.0f;  // Zoom in
    else
        zoom += 5.0f;  // Zoom out

    zoom = std::clamp(zoom, 10.0f, 1000.0f); // Prevent extreme zoom
    update();
}

// Paint GL
void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -zoom);

    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

    QVector3D center = computeCenter();
    glTranslatef(-center.x(), -center.y(), -center.z());
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow

    glBegin(GL_LINES);
    for (size_t i = 0; i + 1 < vertices.size(); i += 2) {
        const auto &v1 = vertices[i];
        const auto &v2 = vertices[i + 1];
        glVertex3f(v1[0], v1[1], v1[2]);
        glVertex3f(v2[0], v2[1], v2[2]);
    }
    qDebug() << "Painting" << vertices.size() << "points";

    glEnd();
}