//
// Created by wzx on 18-12-2.
//
#include <gtest/gtest.h>
#include <vtkSphereSource.h>
#include <vector>
#include <vtkArrowSource.h>

#include "../../util/LineUtil.h"
#include "../../include/STLRender.h"

using std::vector;


class LineUtilTest : public ::testing::Test {

protected:
    vtkSmartPointer<STLRender> stlRender;

    virtual void SetUp() {
        stlRender = vtkSmartPointer<STLRender>::New();
        stlRender->setPath("res/test2.stl");
        stlRender->load();
    }
};


TEST_F(LineUtilTest, lineBlend) {
    array<double, 3> point1 = {-8, 16., -0.4};
    array<double, 3> vector1 = {0.866, -0.5, 0};
    array<double, 3> point2 = {8, 16, -0.4};
    array<double, 3> vector2 = {-0.866, -0.5, 0};
    auto data = LineUtil::lineBlend(point1, vector1, point2, vector2, 50);
    auto points = data->GetPoints();


    vector<vtkSmartPointer<vtkPolyData>> dataList;
    for (int i = 0; i < 2; i++) {
        auto sphere = vtkSmartPointer<vtkSphereSource>::New();
        if (i == 0) {
            sphere->SetCenter(point1.data());
        } else {
            sphere->SetCenter(point2.data());
        }

        sphere->SetRadius(0.5);
        sphere->Update();
        dataList.emplace_back(sphere->GetOutput());
    }
    for (int i = 0; i < points->GetNumberOfPoints(); i++) {
        auto sphere = vtkSmartPointer<vtkSphereSource>::New();
        array<double, 3> point{};
        points->GetPoint(i, point.data());
        sphere->SetCenter(point.data());
        sphere->SetRadius(0.1);
        sphere->Update();
        dataList.emplace_back(sphere->GetOutput());
    }

    stlRender->setInputData(dataList, 0.5);
    stlRender->axisOn();
    stlRender->start();

}