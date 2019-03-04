//
// Created by wzx on 18-12-3.
//
#include <gtest/gtest.h>
#include <vtkSphereSource.h>
#include "../../include/STLRender.h"
#include "../../util/TubeUtil.h"
#include "../../include/Graph.h"

using std::vector;
// vtkbool
int Point::_tag = 0;

class TubeUtilTest : public ::testing::Test {

protected:

    vtkSmartPointer<STLRender> stlRender;

    virtual void SetUp() {
        stlRender = vtkSmartPointer<STLRender>::New();
        stlRender->setPath("res/test2.stl");
        stlRender->load();
    }
};


TEST_F(TubeUtilTest, connetTest) {

    array<double, 3> point1{0.001,10.122,100.000};
    array<double, 3> point2{-0.000,-10.128,100.000};

    array<double, 3> point3{10.125,-0.003,99.995};
    array<double, 3> point4{-10.124,-0.002,100.005};

    array<double, 3> point5{7.160,7.157,100.000};
    array<double, 3> point6{-7.159,-7.162,100.000};

    auto c1 = TubeUtil::createTube(point1, point2, 3, 20);
    auto c2 = TubeUtil::createTube(point3, point4, 3, 20);
    auto c3 = TubeUtil::createTube(point5, point6, 3, 20);

    vector<vtkSmartPointer<vtkPolyData>> data;
    data.emplace_back(c1);
    data.emplace_back(c2);
    data.emplace_back(c3);

    auto graph = TubeUtil::connect(data);

    this->stlRender->setInputData(graph, 1);
    this->stlRender->axisOn();
    this->stlRender->start();
}
