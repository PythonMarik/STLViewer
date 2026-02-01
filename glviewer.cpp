#include "glviewer.h"

#include <QDebug>
#include <QtMath>
#include <QVector3D>
#include <QMatrix4x4>

GLViewer::GLViewer(QWidget* parent)
    : QOpenGLWidget(parent), m_VBO(QOpenGLBuffer::VertexBuffer)
{
    m_model.setToIdentity();
    m_view.setToIdentity();
    m_projection.setToIdentity();

    m_YAW = 0.0f;
    m_PITCH = 20.0f;
    m_DISTANCE = 3;

    m_lastMousePos = {0,0};
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

    static const GLfloat cubeVertices[] = {
        // Front
        -0.5f,-0.5f, 0.5f,   0.5f,-0.5f, 0.5f,   0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,   0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,

        // Back
        0.5f,-0.5f,-0.5f,  -0.5f,-0.5f,-0.5f,  -0.5f, 0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,  -0.5f, 0.5f,-0.5f,   0.5f, 0.5f,-0.5f,

        // Left
        -0.5f,-0.5f,-0.5f,  -0.5f,-0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,  -0.5f, 0.5f, 0.5f,  -0.5f, 0.5f,-0.5f,

        // Right
        0.5f,-0.5f, 0.5f,   0.5f,-0.5f,-0.5f,   0.5f, 0.5f,-0.5f,
        0.5f,-0.5f, 0.5f,   0.5f, 0.5f,-0.5f,   0.5f, 0.5f, 0.5f,

        // Top
        -0.5f, 0.5f, 0.5f,   0.5f, 0.5f, 0.5f,   0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,   0.5f, 0.5f,-0.5f,  -0.5f, 0.5f,-0.5f,

        // Bottom
        -0.5f,-0.5f,-0.5f,   0.5f,-0.5f,-0.5f,   0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,   0.5f,-0.5f, 0.5f,  -0.5f,-0.5f, 0.5f
    };

    // VAO
    m_VAO.create();
    m_VAO.bind();

    // VBO
    m_VBO.create();
    m_VBO.bind();
    m_VBO.allocate(cubeVertices, sizeof(cubeVertices));

    // POS
    m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
    m_program.enableAttributeArray(0);
}

void GLViewer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 1.0, zFar = 13.0, fov = 45.0;

    m_projection.setToIdentity();
    m_projection.perspective(fov, aspect, zNear, zFar);
}

void GLViewer::paintGL()
{
    glClearColor(0.18f, 0.184f, 0.188f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // model
    QVector3D axisY(0.0f, 1.0f, 0.0f);

    // view
    QVector3D eye;
    QVector3D center(0.0f, 0.0f, 0.0f);
    QVector3D up(0.0f, 1.0f, 0.0f);
    float yawRad = qDegreesToRadians(m_YAW);
    float pitchRad = qDegreesToRadians(m_PITCH);
    eye.setX(m_DISTANCE * cos(pitchRad) * sin(yawRad));
    eye.setY(m_DISTANCE * sin(pitchRad));
    eye.setZ(m_DISTANCE * cos(pitchRad) * cos(yawRad));
    m_view.setToIdentity();
    m_view.lookAt(eye, center, up);

    // MVP
    QMatrix4x4 mvp = m_projection * m_view * m_model;

    m_program.bind();
    m_program.setUniformValue("mvp", mvp);
    m_VAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    m_program.release();
}

void GLViewer::mousePressEvent(QMouseEvent* e)
{
    m_lastMousePos = e->pos();
}

void GLViewer::mouseMoveEvent(QMouseEvent* e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        QPoint diff = e->pos() - m_lastMousePos;
        float sens = 360.0f / width();

        m_YAW -= diff.x() * sens;
        m_PITCH += diff.y() * sens;

        m_PITCH = qBound(-89.0f, m_PITCH, 89.0f);

        update();
    }

    m_lastMousePos = e->pos();
}

void GLViewer::scrollWithDegrees(const QPoint& numSteps)
{
    float zoomSpeed = 0.2f;
    m_DISTANCE -= numSteps.y() * zoomSpeed;
    m_DISTANCE = qBound(0.5f, m_DISTANCE, 15.0f);

    update();
}

void GLViewer::wheelEvent(QWheelEvent* e)
{
    QPoint numDegrees = e->angleDelta() / 8;

    if (!numDegrees.isNull())
    {
        QPoint numSteps = numDegrees / 15;
        scrollWithDegrees(numSteps);
    }

    e->accept();
}
