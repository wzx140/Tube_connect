#include <vtkSmartPointer.h>
#include <vtkDelaunay3D.h>
#include <vtkSurfaceReconstructionFilter.h>
#include <vtkContourFilter.h>
#include <time.h>
#include "../include/CellPickerInteractorStyle.h"
#include "../util/TubeUtil.h"

int main(int argc, char **argv) {

//    time of run
    clock_t startTime, endTime;
    startTime = clock();

    vtkSmartPointer<STLRender> stlRender = vtkSmartPointer<STLRender>::New();
    stlRender->setPath("test/res/test2.stl");
    stlRender->load();

//    generate normal and edge points
    auto tubes = stlRender->getTubes();

    for (int i = 0; i < tubes.size(); i++) {
        int index = TubeUtil::next(i, tubes.size(), 0);
        auto point = tubes[i]->getPoints()[0];
        tubes[i]->update(point);
    }

//    generate lines to connect and sample the points on them
    vector<vtkSmartPointer<vtkPolyData>> dataList;
    auto data1 = TubeUtil::connect(tubes[0]->getEdgePoints(), tubes[0]->getNormal(), tubes[1]->getEdgePoints(),
                                   tubes[1]->getNormal(), 0.5, 50);
    auto data2 = TubeUtil::connect(tubes[0]->getEdgePoints(), tubes[0]->getNormal(), tubes[2]->getEdgePoints(),
                                   tubes[2]->getNormal(), 0.5, 50);
    auto data3 = TubeUtil::connect(tubes[1]->getEdgePoints(), tubes[1]->getNormal(), tubes[2]->getEdgePoints(),
                                   tubes[2]->getNormal(), 0.5, 50);
    dataList.insert(dataList.end(), data1.begin(), data1.end());
    dataList.insert(dataList.end(), data2.begin(), data2.end());
    dataList.insert(dataList.end(), data3.begin(), data3.end());
    auto data = STLRender::append(dataList);


    auto surf = vtkSmartPointer<vtkSurfaceReconstructionFilter>::New();
    surf->SetInputData(data);
    surf->Update();
    auto contour = vtkSmartPointer<vtkContourFilter>::New();
    contour->SetInputConnection(surf->GetOutputPort());
    contour->SetValue(0, 0.0);
    contour->Update();


    dataList.clear();
    dataList.emplace_back(contour->GetOutput());
    dataList.emplace_back(stlRender->getData());
    stlRender->setInputData(dataList, 1);
//    stlRender->axisOn();
    stlRender->start();

    endTime = clock();
    cout << "Totle Time : " << (double) (endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
}

