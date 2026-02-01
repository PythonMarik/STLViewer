#ifndef GLVIEWER_H
#define GLVIEWER_H

#include <QObject>

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <QWheelEvent>

class GLViewer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    explicit GLViewer(QWidget* parent = nullptr);
    ~GLViewer();

protected:
    // Функции Qt для OpenGL
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // Обработчики мышки Qt
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;

    // Колесико мыши Qt
    void scrollWithDegrees(const QPoint& numSteps);
    void wheelEvent(QWheelEvent* e) override;

private:
    // Шейдерная программа и буферы
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;

    // MVP
    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    // Параметры для камеры
    float m_YAW;
    float m_PITCH;
    float m_DISTANCE;

    QPoint m_lastMousePos;
};

#endif // GLVIEWER_H
