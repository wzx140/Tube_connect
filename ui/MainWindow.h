//
// Created by wzx on 19-2-18.
//

#ifndef TUBE_CONNECT_MAINWINDOW_H
#define TUBE_CONNECT_MAINWINDOW_H

#include <QMainWindow>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(vtkSmartPointer<vtkRenderWindow> window, QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;


};


#endif //TUBE_CONNECT_MAINWINDOW_H
