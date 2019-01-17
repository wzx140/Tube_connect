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
    graph->setCoefficient3(0.1);
    graph->create(tubes);
    graph->update();

    auto data = STLRender::append(graph->getOutput(1));

    auto surf = vtkSmartPointer<vtkSurfaceReconstructionFilter>::New();
    surf->SetInputData(data);
    surf->Update();
    auto contour = vtkSmartPointer<vtkContourFilter>::New();
    contour->SetInputConnection(surf->GetOutputPort());
    contour->SetValue(0, 0.0);
    contour->Update();


    vector<vtkSmartPointer<vtkPolyData>> dataList;
    dataList.emplace_back(contour->GetOutput());
//    dataList.emplace_back(data);
    dataList.emplace_back(graph->getOutput(2)[0]);
    stlRender->setInputData(dataList, 1);
//    stlRender->axisOn();
    stlRender->start();

    endTime = clock();
    cout << "Totle Time : " << (double) (endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
}

