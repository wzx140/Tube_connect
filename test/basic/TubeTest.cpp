//
// Created by wzx on 18-11-21.
//
#include <gtest/gtest.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vector>
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

TEST_F(TubeTest, hasPointTest) {
    array<double, 3> point1 = {-25.246, 9.1294, 8.8177};
    array<double, 3> point2 = {-20.413, 11.786, 10};
    array<double, 3> point3 = {-21.511, 4.17, 6.997};
    array<double, 3> point4 = {16.593, 6.975, 9.7423};

    stlRender->setPath("res/test2.stl");
    stlRender->load();
    auto tubes = stlRender->getTubes();

    EXPECT_TRUE(tubes[0]->hasPoint(point1));
    EXPECT_TRUE(tubes[0]->hasPoint(point2));
    EXPECT_TRUE(tubes[0]->hasPoint(point3));
    EXPECT_FALSE(tubes[0]->hasPoint(point4));
    EXPECT_TRUE(tubes[1]->hasPoint(point4));
}

TEST_F(TubeTest, updateNormalTest) {
    array<double, 3> point1 = {10, 100, 100};
    array<double, 3> point2 = {10, 100, -100};

    stlRender->setPath("res/test1.stl");
    stlRender->load();
    auto tubes = stlRender->getTubes();
    EXPECT_EQ(tubes.size(), 1);
    auto tube = tubes[0];
    tube->update(point1);

    EXPECT_EQ(tube->getNormal()[0], 0);
    EXPECT_EQ(tube->getNormal()[1], 0);
    EXPECT_EQ(tube->getNormal()[2], 1);

    tube->update(point2);

    EXPECT_EQ(tube->getNormal()[0], 0);
    EXPECT_EQ(tube->getNormal()[1], 0);
    EXPECT_EQ(tube->getNormal()[2], -1);
}

/**
 * if you see lots of points on the edge of tube, the test pass
 */
TEST_F(TubeTest, updateEdgeTest) {
    array<double, 3> point1 = {0, 0, 50};

    stlRender->setPath("res/test2.stl");
    stlRender->load();
    auto tubes = stlRender->getTubes();
    auto tube = tubes[0];
    array<double, 3> point{0, 0, 0};
    tubes[1]->getPoints()->GetTuple(5, point.data());
    tube->update(point);
    auto points = tube->getEdgePoints();
    vector<vtkSmartPointer<vtkPolyData>> dataList;
    dataList.push_back(stlRender->getData());
    for (int i = 0; i < points->GetNumberOfTuples(); i++) {
        auto sphere = vtkSmartPointer<vtkSphereSource>::New();
        sphere->SetCenter(points->GetTuple3(i));
        sphere->SetRadius(0.1);
        sphere->Update();
        dataList.emplace_back(sphere->GetOutput());
    }
    stlRender->setInputData(dataList, 0.5);
    stlRender->axisOn();
    stlRender->start();

}