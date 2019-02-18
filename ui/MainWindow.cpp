//
// Created by wzx on 19-2-18.
//

#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(vtkSmartPointer<vtkRenderWindow> window, QWidget *parent) : QMainWindow(parent),
                                                                                   ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->statusbar->showMessage("Made by WZX");
    ui->openGLWidget->SetRenderWindow(window);
}

MainWindow::~MainWindow() {
    delete ui;
}
