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
    vtkSmartPointer<Tube> filter;
    vtkSmartPointer<STLRender> stlRender;

    virtual void SetUp() {
        this->filter = vtkSmartPointer<Tube>::New();
        this->stlRender = vtkSmartPointer<STLRender>::New();
    }
};


TEST_F(TubeTest, hasPointTest) {
    double point1[3] = {-25.246, 9.1294, 8.8177};
    double point2[3] = {-20.413, 11.786, 10};
    double point3[3] = {-21.511, 4.17, 6.997};
    double point4[3] = {16.593, 6.975, 9.7423};

    stlRender->setPath("../test/res/test2.stl");
    stlRender->load();
    auto tubes = stlRender->getTubes();

    EXPECT_TRUE(tubes.at(0)->hasPoint(point1));
    EXPECT_TRUE(tubes.at(0)->hasPoint(point2));
    EXPECT_TRUE(tubes.at(0)->hasPoint(point3));
    EXPECT_FALSE(tubes.at(0)->hasPoint(point4));
    EXPECT_TRUE(tubes.at(1)->hasPoint(point4));
}

TEST_F(TubeTest, updateNormalTest) {
    double point1[3] = {0, 0, 50};
    double point2[3] = {0, 0, -50};

    stlRender->setPath("../test/res/test1.stl");
    stlRender->load();
    auto tubes = stlRender->getTubes();
    EXPECT_EQ(tubes.size(), 1);
    auto tube = tubes.at(0);
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
    double point1[3] = {0, 0, 50};

    stlRender->setPath("../test/res/test1.stl");
    stlRender->load();
    auto tubes = stlRender->getTubes();
    EXPECT_EQ(tubes.size(), 1);
    auto tube = tubes.at(0);
    tube->update(point1);
    auto points = tube->getEdgePoints();
    vector<vtkSmartPointer<vtkPolyData>> dataList;
    dataList.push_back(stlRender->getData());
    for (int i = 0; i < points->GetNumberOfTuples(); i++) {
        auto sphere = vtkSmartPointer<vtkSphereSource>::New();
        sphere->SetCenter(points->GetTuple3(i));
        sphere->Update();
        dataList.push_back(sphere->GetOutput());
    }
    stlRender->setInputData(dataList);
    stlRender->start();

}