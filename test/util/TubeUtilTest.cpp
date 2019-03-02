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
    auto l1 = vtkSmartPointer<vtkLineSource>::New();
    auto l2 = vtkSmartPointer<vtkLineSource>::New();

    array<double, 3> point1{64.926935789806464, 7.1501248777546049, -0.00025614178654999461};
    array<double, 3> point2{50.395716061874381, -7.1503924093412774, -0.00017248867591595495};
    array<double, 3> point3{47.467441221438307, -0.00013376573518674223, -0.00020437462342637406};
    array<double, 3> point4{67.855210630242539, -0.00013376585148573366, -0.00022425583903957553};

    auto c1 = TubeUtil::createTube(point1, point2, 3, 20);
    auto c2 = TubeUtil::createTube(point3, point4, 3, 20);

    vector<vtkSmartPointer<vtkPolyData>> data;
    data.emplace_back(c1);
    data.emplace_back(c2);

    auto graph = TubeUtil::connect(data);

    this->stlRender->setInputData(graph, 1);
    this->stlRender->axisOn();
    this->stlRender->start();
}
