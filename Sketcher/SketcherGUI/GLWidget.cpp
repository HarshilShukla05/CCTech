#include "GLWidget.h"
#include <QDebug>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent) {}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f); // blueish background
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 5.0, 500.0);  // FOV, aspect, near, far
    glMatrixMode(GL_MODELVIEW);
    qDebug() << "OpenGL initialized";
}

void GLWidget::setShapeVertices(const std::vector<std::vector<double>> &verts) {
    vertices = verts;
    update();
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-15.0f, -15.0f, -100.0f);

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