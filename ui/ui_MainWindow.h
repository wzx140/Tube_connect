/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QVTKOpenGLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QVTKOpenGLWidget *openGLWidget;
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;
    QGridLayout *gridLayout_2;
    QPushButton *importButton;
    QPushButton *runButton;
    QPushButton *exportButton;
    QPushButton *clearButton;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1000, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        openGLWidget = new QVTKOpenGLWidget(centralwidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy);
        openGLWidget->setMinimumSize(QSize(600, 400));

        gridLayout->addWidget(openGLWidget, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableWidget = new QTableWidget(centralwidget);
        if (tableWidget->columnCount() < 1)
            tableWidget->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setText(QString::fromUtf8("Value"));
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        if (tableWidget->rowCount() < 5)
            tableWidget->setRowCount(5);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(1, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(2, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(3, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(4, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        __qtablewidgetitem6->setFlags(
                Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsUserCheckable |
                Qt::ItemIsEnabled);
        tableWidget->setItem(0, 0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        __qtablewidgetitem7->setFlags(
                Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsUserCheckable |
                Qt::ItemIsEnabled);
        tableWidget->setItem(1, 0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        __qtablewidgetitem8->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        tableWidget->setItem(2, 0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        __qtablewidgetitem9->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        tableWidget->setItem(3, 0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        __qtablewidgetitem10->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        tableWidget->setItem(4, 0, __qtablewidgetitem10);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy1);
        tableWidget->setMinimumSize(QSize(380, 481));
        tableWidget->setAcceptDrops(false);
        tableWidget->setAutoFillBackground(false);
        tableWidget->setSortingEnabled(false);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(false);
        tableWidget->horizontalHeader()->setDefaultSectionSize(290);
        tableWidget->verticalHeader()->setCascadingSectionResizes(false);

        verticalLayout->addWidget(tableWidget);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        importButton = new QPushButton(centralwidget);
        importButton->setObjectName(QString::fromUtf8("importButton"));

        gridLayout_2->addWidget(importButton, 3, 0, 1, 1);

        runButton = new QPushButton(centralwidget);
        runButton->setObjectName(QString::fromUtf8("runButton"));
        runButton->setEnabled(false);

        gridLayout_2->addWidget(runButton, 2, 0, 1, 1);

        exportButton = new QPushButton(centralwidget);
        exportButton->setObjectName(QString::fromUtf8("exportButton"));
        exportButton->setEnabled(false);

        gridLayout_2->addWidget(exportButton, 3, 1, 1, 1);

        clearButton = new QPushButton(centralwidget);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setEnabled(false);

        gridLayout_2->addWidget(clearButton, 2, 1, 1, 1);


        verticalLayout->addLayout(gridLayout_2);


        gridLayout->addLayout(verticalLayout, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(importButton, SIGNAL(clicked()), MainWindow, SLOT(import()));
        QObject::connect(runButton, SIGNAL(clicked()), MainWindow, SLOT(run()));
        QObject::connect(clearButton, SIGNAL(clicked()), MainWindow, SLOT(clear()));
        QObject::connect(exportButton, SIGNAL(clicked()), MainWindow, SLOT(output()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Tube Connection", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "file", nullptr));
#ifndef QT_NO_TOOLTIP
        ___qtablewidgetitem->setToolTip(QApplication::translate("MainWindow", "path of file", nullptr));
#endif // QT_NO_TOOLTIP
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "length", nullptr));
#ifndef QT_NO_TOOLTIP
        ___qtablewidgetitem1->setToolTip(
                QApplication::translate("MainWindow", "the length of each tube after connection", nullptr));
#endif // QT_NO_TOOLTIP
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "coefficient1", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->verticalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "coefficient2", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->verticalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "coefficient3", nullptr));

        const bool __sortingEnabled = tableWidget->isSortingEnabled();
        tableWidget->setSortingEnabled(false);
        tableWidget->setSortingEnabled(__sortingEnabled);

        importButton->setText(QApplication::translate("MainWindow", "IMPORT", nullptr));
        runButton->setText(QApplication::translate("MainWindow", "RUN", nullptr));
        exportButton->setText(QApplication::translate("MainWindow", "EXPORT", nullptr));
        clearButton->setText(QApplication::translate("MainWindow", "CLEAR", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
