#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "glviewer.h"

#include <QOpenGLWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("STL Viewer");

    // openGL
    auto glViewer = new GLViewer(this);
    ui->verticalLayout_GLViewer->addWidget(glViewer);
}

MainWindow::~MainWindow()
{
    delete ui;
}
