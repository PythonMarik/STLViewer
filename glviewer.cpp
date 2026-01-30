#include "glviewer.h"

GLViewer::GLViewer(QWidget* parent)
    : QOpenGLWidget(parent)
{

}

GLViewer::~GLViewer()
{

}

void GLViewer::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.18f, 0.184f, 0.188f, 1.0f);
}

void GLViewer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLViewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
