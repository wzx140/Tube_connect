#include <QApplication>
#include <ui/MainWindow.h>
#include <QtGui/qsurfaceformat.h>
#include <QVTKOpenGLWidget.h>



int main(int argc, char **argv) {
    auto stlRender = vtkSmartPointer<STLRender>::New();
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());
    QApplication a(argc, argv);
    MainWindow w(stlRender);
    w.show();
    return a.exec();
}