//
// Created by wzx on 18-12-3.
//
#include <gtest/gtest.h>
#include <vtkSphereSource.h>
#include "../../include/STLRender.h"
#include "../../util/TubeUtil.h"

using std::vector;

class TubeUtilTest : public ::testing::Test {

protected:

    vtkSmartPointer<STLRender> stlRender;

    virtual void SetUp() {
        stlRender = vtkSmartPointer<STLRender>::New();
        stlRender->setPath("res/test2.stl");
        stlRender->load();
    }
};

TEST_F(TubeUtilTest, connectTest) {
    auto tubes = stlRender->getTubes();
    for (int i = 0; i < tubes.size(); i++) {
        int index = i + 1;
        if (i == tubes.size() - 1) {
            index = 0;
        }
        auto point = tubes[index]->getPoints()[5];
        tubes[i]->update(point);
    }

    vector<vtkSmartPointer<vtkPolyData>> source;
    vector<vtkSmartPointer<vtkPolyData>> dataList;
    auto data1 = TubeUtil::connect(tubes[0]->getEdgePoints(), tubes[0]->getNormal(), tubes[1]->getEdgePoints(),
                                   tubes[1]->getNormal(), 0.5, 50);
    auto data2 = TubeUtil::connect(tubes[0]->getEdgePoints(), tubes[0]->getNormal(), tubes[2]->getEdgePoints(),
                                   tubes[2]->getNormal(), 0.5, 50);
    auto data3 = TubeUtil::connect(tubes[1]->getEdgePoints(), tubes[1]->getNormal(), tubes[2]->getEdgePoints(),
                                   tubes[2]->getNormal(), 0.5, 50);

    source.insert(source.end(), data1.begin(), data1.end());
    source.insert(source.end(), data2.begin(), data2.end());
    source.insert(source.end(), data3.begin(), data3.end());
    for (int i = 0; i < source.size(); i++) {
        auto points = source[i]->GetPoints();
        for (int i = 0; i < points->GetNumberOfPoints(); i++) {
            auto sphere = vtkSmartPointer<vtkSphereSource>::New();
            array<double, 3> point{};
            points->GetPoint(i, point.data());
            sphere->SetCenter(point.data());
            sphere->SetRadius(0.2);
            sphere->Update();
            dataList.emplace_back(sphere->GetOutput());
        }

    }
    dataList.push_back(stlRender->getData());
    for (int i = 0; i < 3; i++) {
        auto points = tubes[i]->getEdgePoints();

        for (int i = 0; i < points->GetNumberOfTuples(); i++) {
            auto sphere = vtkSmartPointer<vtkSphereSource>::New();
            sphere->SetCenter(points->GetTuple3(i));
            sphere->SetRadius(0.5);
            sphere->Update();
            dataList.emplace_back(sphere->GetOutput());
        }
    }
    this->stlRender->setInputData(dataList, 1);
    this->stlRender->axisOn();
    this->stlRender->start();


}