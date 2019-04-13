//
// Created by wzx on 18-11-21.
//
#include <gtest/gtest.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vector>
#include <vtkLine.h>
#include <vtkPolyDataMapper.h>
#include "../../include/Tube.h"
#include "../../include/STLRender.h"

using namespace std;

class TubeTest : public ::testing::Test {

protected:
    vtkSmartPointer<STLRender> stlRender;

    virtual void SetUp() {
        this->stlRender = vtkSmartPointer<STLRender>::New();
    }
};

/**
 * if you see the points on the tube, the test pass
 */
TEST_F(TubeTest, getPointsTest) {
    stlRender->setPath("../res/test1.stl");
    stlRender->load();
    auto tubes = stlRender->getTubes();
    auto tube = tubes[0];
    auto points = tube->getPoints();
    vector<vtkSmartPointer<vtkPolyData>> dataList;
    dataList.push_back(stlRender->getData());
    for (auto &point : points) {
        auto sphere = vtkSmartPointer<vtkSphereSource>::New();
        sphere->SetCenter(point.data());
        sphere->SetRadius(0.2);
        sphere->Update();
        dataList.emplace_back(sphere->GetOutput());
    }
    stlRender->setInputData(dataList, 0.5);
    stlRender->axisOn();
    stlRender->start();
}


/**
 * if you see a cylinder and its structure line, the test pass
 */
TEST_F(TubeTest, getStructureLineTest) {

    stlRender->setPath("../res/test2.stl");
    stlRender->load();
    auto tubes = stlRender->getTubes();
    auto tube = tubes[0];
    auto line = tube->getStructureLine();
    vector<vtkSmartPointer<vtkPolyData>> dataList;

    auto data = vtkSmartPointer<vtkPolyData>::New();
    auto points = vtkSmartPointer<vtkPoints>::New();
    auto line_ = vtkSmartPointer<vtkLine>::New();
    auto lines_ = vtkSmartPointer<vtkCellArray>::New();
    points->SetNumberOfPoints(2);
    points->SetPoint(0, line[0].data());
    points->SetPoint(1, line[1].data());
    line_->GetPointIds()->SetId(0, 0);
    line_->GetPointIds()->SetId(1, 1);
    lines_->InsertNextCell(line_);
    data->SetPoints(points);
    data->SetLines(lines_);
    dataList.emplace_back(data);
    dataList.emplace_back(tube->getData());


    stlRender->setInputData(dataList, 0.5);
    stlRender->axisOn();
    stlRender->start();

}

