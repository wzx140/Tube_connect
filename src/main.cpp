#include <vtkSmartPointer.h>
#include <vtkDelaunay3D.h>
#include <vtkSurfaceReconstructionFilter.h>
#include <vtkContourFilter.h>
#include "../include/CellPickerInteractorStyle.h"
#include "../util/TubeUtil.h"
#include "../include/Graph.h"

int main(int argc, char **argv) {

//    time of run
    clock_t startTime, endTime;
    startTime = clock();

    auto stlRender = vtkSmartPointer<STLRender>::New();
    stlRender->setPath("test/res/test3.stl");
    stlRender->load();

    auto graph = vtkSmartPointer<Graph>::New();
    auto tubes = stlRender->getTubes();
    graph->setLength(200);
    graph->setRadius(5);
    graph->setCoefficient3(0.2);
    graph->create(tubes);
    graph->update();

    vector<vtkSmartPointer<vtkPolyData>> dataList;
    for (int i = 1; i < graph->getIntersections().size() + 1; i++) {
        auto surf = vtkSmartPointer<vtkSurfaceReconstructionFilter>::New();
        surf->SetInputData(graph->getOutput(i));
        surf->Update();
        auto contour = vtkSmartPointer<vtkContourFilter>::New();
        contour->SetInputConnection(surf->GetOutputPort());
        contour->SetValue(0, 0.0);
        contour->Update();
        dataList.emplace_back(contour->GetOutput());
    }

    dataList.emplace_back(graph->getOutput(0));
    stlRender->setInputData(dataList, 1);
//    stlRender->axisOn();
    stlRender->start();

    endTime = clock();
    cout << "Total Time : " << (double) (endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
}

