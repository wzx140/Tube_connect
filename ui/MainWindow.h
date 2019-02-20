//
// Created by wzx on 19-2-18.
//

#ifndef TUBE_CONNECT_MAINWINDOW_H
#define TUBE_CONNECT_MAINWINDOW_H

#include <QMainWindow>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>

#include "../include/STLRender.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(vtkSmartPointer<STLRender> stlRender, QWidget *parent = nullptr);

    ~MainWindow();

private slots:

    /**
     * import stl file
     */
    void import();

    /**
     * connect tubes
     */
    void run();

    /**
     * clear the tube
     */
    void clear();

    /**
     * export the stl file
     */
    void output();

private:
    vtkSmartPointer<STLRender> stlRender;
    Ui::MainWindow *ui;


};


#endif //TUBE_CONNECT_MAINWINDOW_H
