//
// Created by wzx on 18-12-2.
//
#include <gtest/gtest.h>
#include <vtkSphereSource.h>
#include <vector>

#include "../../util/LineUtil.h"
#include "../../include/STLRender.h"
#include "../../util/CircleUtil.h"

using std::vector;


class LineUtilTest : public ::testing::Test {

protected:
    vtkSmartPointer<STLRender> stlRender;

    virtual void SetUp() {
        stlRender = vtkSmartPointer<STLRender>::New();
        stlRender->setPath("../res/test2.stl");
        stlRender->load();
    }
};

TEST_F(LineUtilTest, intersection3DTest) {
    array<double, 3> point11 = {1, 1, 0};
    array<double, 3> point12 = {-1, -1, 0};
    array<double, 3> point21 = {1, -1, 0};
    array<double, 3> point22 = {-1, 1, 0};
    array<double, 3> point{-1, -1, -1};
    int state = LineUtil::intersection3D(point11, point12, point21, point22, point);
    EXPECT_EQ(state, 1);
    if (state == 1) {
        EXPECT_EQ(point[0], 0);
        EXPECT_EQ(point[1], 0);
        EXPECT_EQ(point[2], 0);
    }

}

/**
 * if you see that a line is long, the test is pass
 */
TEST_F(LineUtilTest, extendTest) {
    array<double, 3> point1 = {-1, -1, -1};
    array<double, 3> point2 = {1, 1, 1};
    array<array<double, 3>, 2> lineP{point1, point2};

    lineP = LineUtil::extend(lineP, 100);

    auto data = vtkSmartPointer<vtkPolyData>::New();
    auto line = vtkSmartPointer<vtkLine>::New();
    auto points = vtkSmartPointer<vtkPoints>::New();
    auto lines = vtkSmartPointer<vtkCellArray>::New();

    points->InsertNextPoint(lineP[0].data());
    points->InsertNextPoint(lineP[1].data());
    line->GetPointIds()->SetId(0, 0);
    line->GetPointIds()->SetId(1, 1);
    lines->InsertNextCell(line);
    data->SetPoints(points);
    data->SetLines(lines);

    stlRender->setInputData(data, 1);
    stlRender->axisOn();
    stlRender->start();
}

/**
 * if you see a line truncated by the origin, the test is pass
 */
TEST_F(LineUtilTest, cutTest) {
    array<double, 3> point11 = {-10, -10, -10};
    array<double, 3> point12 = {10, 10, 10};
    array<double, 3> point{0, 0, 0};
    auto lines = LineUtil::cut(point11, point12, point, 5);
    auto data = vtkSmartPointer<vtkPolyData>::New();
    auto line1 = vtkSmartPointer<vtkLine>::New();
    auto line2 = vtkSmartPointer<vtkLine>::New();
    auto points = vtkSmartPointer<vtkPoints>::New();
    auto lines_ = vtkSmartPointer<vtkCellArray>::New();
    points->InsertNextPoint(lines.at(0).data());
    points->InsertNextPoint(lines.at(1).data());
    points->InsertNextPoint(lines.at(2).data());
    points->InsertNextPoint(lines.at(3).data());
    line1->GetPointIds()->SetId(0, 0);
    line1->GetPointIds()->SetId(1, 1);
    line2->GetPointIds()->SetId(0, 2);
    line2->GetPointIds()->SetId(1, 3);
    lines_->InsertNextCell(line1);
    lines_->InsertNextCell(line2);
    data->SetLines(lines_);
    data->SetPoints(points);
    stlRender->setInputData(data, 1);
    stlRender->axisOn();
    stlRender->start();
}

/**
 * if you see a line truncated by the some points, the test is pass
 */
TEST_F(LineUtilTest, cut2Test) {
    array<double, 3> point11 = {-10, -10, -10};
    array<double, 3> point12 = {10, 10, 10};

    vector<array<double, 3>> points_;
    array<double, 3> point1{-1, -1, -1};
    array<double, 3> point2{3, 3, 3};
    array<double, 3> point3{7, 7, 7};
    points_.emplace_back(point2);
    points_.emplace_back(point1);
    points_.emplace_back(point3);

    vector<double> a{2, 2, 2};
    auto lines = LineUtil::cut(point11, point12, points_, a);

    auto data = vtkSmartPointer<vtkPolyData>::New();
    auto points = vtkSmartPointer<vtkPoints>::New();
    auto lines_ = vtkSmartPointer<vtkCellArray>::New();

    for (int i = 0; i < lines.size(); i++) {
        auto line = vtkSmartPointer<vtkLine>::New();

        points->InsertNextPoint(lines.at(i).at(0).data());
        points->InsertNextPoint(lines.at(i).at(1).data());

        line->GetPointIds()->SetId(0, i * 2);
        line->GetPointIds()->SetId(1, i * 2 + 1);

        lines_->InsertNextCell(line);
    }
    data->SetLines(lines_);
    data->SetPoints(points);

    stlRender->setInputData(data, 1);
//    stlRender->axisOn();
    stlRender->start();
}