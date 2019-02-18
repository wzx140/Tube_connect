#include <vtkSmartPointer.h>
#include <vtkDelaunay3D.h>
#include <vtkSurfaceReconstructionFilter.h>
#include <vtkContourFilter.h>
#include <vtkReverseSense.h>
#include <QApplication>
#include <ui/MainWindow.h>

#include "../include/CellPickerInteractorStyle.h"
#include "../util/TubeUtil.h"
#include "../include/Graph.h"

//int main(int argc, char **argv) {
//
////    time of run
//    clock_t startTime, endTime;
//    startTime = clock();
//
//    auto stlRender = vtkSmartPointer<STLRender>::New();
//    stlRender->setPath("test/res/test3.stl");
//    stlRender->load();
//
//    auto graph = vtkSmartPointer<Graph>::New();
//    auto tubes = stlRender->getTubes();
//    graph->setLength(250);
//    graph->setRadius(3);
//    graph->setCoefficient3(0.2);
//    graph->create(tubes);
//    graph->update();
//
//    vector<vtkSmartPointer<vtkPolyData>> dataList;
//    for (int i = 1; i < graph->getIntersections().size() + 1; i++) {
//        auto surf = vtkSmartPointer<vtkSurfaceReconstructionFilter>::New();
//        surf->SetInputData(graph->getOutput(i));
//        // the bigger value, the more time
//        surf->SetNeighborhoodSize(30);
//        surf->Update();
//        auto contour = vtkSmartPointer<vtkContourFilter>::New();
//        contour->SetInputConnection(surf->GetOutputPort());
//        contour->SetValue(0, 0.0);
//        auto reverse = vtkSmartPointer<vtkReverseSense>::New();
//        reverse->SetInputConnection(contour->GetOutputPort());
//        reverse->ReverseCellsOn();
//        reverse->ReverseNormalsOn();
//        reverse->Update();
//        dataList.emplace_back(reverse->GetOutput());
//    }
//
//    dataList.emplace_back(graph->getOutput(0));
//    stlRender->setInputData(dataList, 1);
////    stlRender->axisOn();
//    stlRender->start();
//
//    endTime = clock();
//    cout << "Total Time : " << (double) (endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
//}

int main(int argc, char **argv) {
    auto stlRender = vtkSmartPointer<STLRender>::New();
    stlRender->setPath("test/res/test3.stl");
    stlRender->load();
    stlRender->setInputData(stlRender->getData(), 0.5);

    QApplication a(argc, argv);
    MainWindow w(stlRender->getWindow());
    w.show();
    return a.exec();
}