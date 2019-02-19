#include <vtkSmartPointer.h>
#include <vtkDelaunay3D.h>
#include <vtkSurfaceReconstructionFilter.h>
#include <vtkContourFilter.h>
#include <vtkReverseSense.h>
#include <QApplication>
#include <ui/MainWindow.h>
#include <QtGui/qsurfaceformat.h>
#include <QVTKOpenGLWidget.h>

#include "../include/CellPickerInteractorStyle.h"
#include "../util/TubeUtil.h"
#include "../include/Graph.h"


int main(int argc, char **argv) {
    auto stlRender = vtkSmartPointer<STLRender>::New();
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());
    QApplication a(argc, argv);
    MainWindow w(stlRender);
    w.show();
    return a.exec();
}