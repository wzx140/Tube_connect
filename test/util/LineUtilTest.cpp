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
    array<double, 3> point1 = {16.302, 1.7644, 7};
    array<double, 3> vector1 = {-0.433, -0.5, 0};
    array<double, 3> point2 = {6.7775, -15, 7};
    array<double, 3> vector2 = {0, 1, 0};
    vtkSmartPointer<vtkPoints> points = LineUtil::lineBlend(point1, vector1, point2, vector2, 100)->GetPoints();

    ::vector<vtkSmartPointer<vtkPolyData>> dataList;
    for (int i = 0; i < 2; i++) {
        auto sphere = vtkSmartPointer<vtkSphereSource>::New();
        if (i == 0) {
            sphere->SetCenter(point1.data());
        } else {
            sphere->SetCenter(point2.data());
        }

        sphere->SetRadius(2);
        sphere->Update();
        dataList.emplace_back(sphere->GetOutput());
    }
    for (int i = 0; i < points->GetNumberOfPoints(); i++) {
        auto sphere = vtkSmartPointer<vtkSphereSource>::New();
        array<double, 3> point{};
        points->GetPoint(i, point.data());
        sphere->SetCenter(point.data());
        sphere->SetRadius(1);
        sphere->Update();
        dataList.emplace_back(sphere->GetOutput());
    }

    stlRender->setInputData(dataList, 1);
    stlRender->start();

}