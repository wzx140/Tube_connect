//
// Created by wzx on 19-2-18.
//
#include <vtkSurfaceReconstructionFilter.h>
#include <vtkContourFilter.h>
#include <qstring.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <QtWidgets/qmessagebox.h>

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "../util/TubeUtil.h"
#include "../include/Graph.h"


MainWindow::MainWindow(vtkSmartPointer<STLRender> stlRender, QWidget *parent) : QMainWindow(parent),
                                                                                ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QLabel *perLabel = new QLabel();
    perLabel->setText("made by WZX");
    perLabel->setOpenExternalLinks(true);
    ui->statusbar->addPermanentWidget(perLabel);
    ui->statusbar->showMessage("null");

    this->stlRender = stlRender;
    ui->openGLWidget->SetRenderWindow(stlRender->getWindow());
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::import() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("import STL file"), "res",
                                                    tr("STLFile(*.stl *.STL)"));
    if (!fileName.isEmpty() && (fileName.endsWith(".stl", Qt::CaseInsensitive))) {
        this->stlRender->setPath(fileName.toStdString());
        this->stlRender->load();
        this->stlRender->setInputData(this->stlRender->getData(), 1);
        ui->openGLWidget->update();

        ui->exportButton->setEnabled(true);
        ui->runButton->setEnabled(true);
        ui->clearButton->setEnabled(true);
        ui->tableWidget->setEnabled(true);

        ui->tableWidget->item(0, 0)->setText(fileName);
        ui->tableWidget->item(0, 0)->setFlags(Qt::ItemIsEnabled);

        ui->tableWidget->item(1, 0)->setText(QString::number(250));
        ui->tableWidget->item(2, 0)->setText(QString::number(1.2));
        ui->tableWidget->item(3, 0)->setText(QString::number(20));
        ui->tableWidget->item(4, 0)->setText(QString::number(30));
        ui->statusbar->showMessage("loaded");
        stlRender->getRenderer()->ResetCamera();
        stlRender->getWindow()->Render();

    }
}

void MainWindow::run() {

    auto lengthStr = ui->tableWidget->item(1, 0)->text();
    auto coefficient1Str = ui->tableWidget->item(2, 0)->text();
    auto coefficient2Str = ui->tableWidget->item(3, 0)->text();
    auto coefficient3Str = ui->tableWidget->item(4, 0)->text();

    double length = lengthStr.toDouble();
    double coefficient1 = coefficient1Str.toDouble();
    double coefficient2 = coefficient2Str.toDouble();
    int coefficient3 = coefficient3Str.toInt();

//    Check input value
    if (length <= 0) {
        QMessageBox::critical(this, "Input value error!", "length error");
        return;
    } else if (coefficient1 <= 0 || coefficient2 <= 0 || coefficient3 <= 0) {
        QMessageBox::critical(this, "Input value error!", "coefficient error");
        return;
    }

    ui->statusbar->showMessage("running");
//    time of run
    clock_t startTime, endTime;
    startTime = clock();

    auto graph = vtkSmartPointer<Graph>::New();
    auto tubes = this->stlRender->getTubes();
    graph->setLength(length);
    graph->setCoefficient1(coefficient1);
    graph->setCoefficient2(coefficient2);
    graph->setCoefficient3(coefficient3);
    graph->create(tubes);
    graph->update();

    stlRender->setInputData(graph->getOutput(), 1);
    endTime = clock();
    ui->statusbar->showMessage("Total time:" + QString::number((double) (endTime - startTime) / CLOCKS_PER_SEC) + "s");
    stlRender->getRenderer()->ResetCamera();
    stlRender->getWindow()->Render();
}

void MainWindow::clear() {
    this->stlRender->setInputData(vtkSmartPointer<vtkPolyData>::New(), 1);
    ui->openGLWidget->update();

    ui->exportButton->setEnabled(false);
    ui->runButton->setEnabled(false);
    ui->clearButton->setEnabled(false);

    ui->tableWidget->item(0, 0)->setText("");
    ui->tableWidget->item(0, 0)->setFlags(Qt::ItemIsEditable);
    ui->tableWidget->item(1, 0)->setText("");
    ui->tableWidget->item(2, 0)->setText("");
    ui->tableWidget->item(3, 0)->setText("");
    ui->tableWidget->item(4, 0)->setText("");
    ui->tableWidget->setEnabled(false);
    ui->statusbar->showMessage("null");
    stlRender->getRenderer()->ResetCamera();
    stlRender->getWindow()->Render();

}

void MainWindow::output() {
    QString path = QFileDialog::getSaveFileName(this, "Save stl file", ".", "*.stl;");
    bool flag = false;
//    whether dir exist
    if (QDir(QFileInfo(path).absolutePath()).exists() && !path.isEmpty()) {
        if (!path.endsWith(".stl") && !path.endsWith(".STL")) {
            path += ".stl";
        }
        flag = this->stlRender->output(path.toStdString());
    }
    if (flag) {
        ui->statusbar->showMessage("save successful");
    } else {
        ui->statusbar->showMessage("save fail");
    }
}
