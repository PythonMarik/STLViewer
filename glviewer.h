#ifndef GLVIEWER_H
#define GLVIEWER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QObject>

class GLViewer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLViewer(QWidget* parent = nullptr);
    ~GLViewer();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

};

#endif // GLVIEWER_H
