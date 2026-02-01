#include "glviewer.h"

#include <QDebug>

GLViewer::GLViewer(QWidget* parent)
    : QOpenGLWidget(parent), m_VBO(QOpenGLBuffer::VertexBuffer)
{

}

GLViewer::~GLViewer()
{
    makeCurrent();

    m_VBO.destroy();
    m_VAO.destroy();

    doneCurrent();
}

void GLViewer::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex.vert"))
        qDebug() << "Ошибка: " << m_program.log();

    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.frag"))
        qDebug() << "Ошибка: " << m_program.log();

    m_program.link();

    static const GLfloat vertices[] = {
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
    };

    // VAO
    m_VAO.create();
    m_VAO.bind();

    // VBO
    m_VBO.create();
    m_VBO.bind();
    m_VBO.allocate(vertices, sizeof(vertices));

    // POS
    m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
    m_program.enableAttributeArray(0);

    // COLOR
    m_program.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    m_program.enableAttributeArray(1);
}

void GLViewer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLViewer::paintGL()
{
    glClearColor(0.18f, 0.184f, 0.188f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program.bind();
    m_VAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    m_program.release();
}
