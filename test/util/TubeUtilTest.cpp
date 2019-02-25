//
// Created by wzx on 18-12-3.
//
#include <gtest/gtest.h>
#include <vtkSphereSource.h>
#include "../../include/STLRender.h"
#include "../../util/TubeUtil.h"
#include "../../include/Graph.h"

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

/**
 * if you see cylinder and its normal and center compliance information, the test is pass
 */
TEST_F(TubeUtilTest, createCylinderTest) {
    array<double, 3> normal = {1, 1, 1};
    array<double, 3> center = {0, 0, 0};
    auto graph = TubeUtil::createCylinder(normal, center, 5, 20, 20);

    this->stlRender->setInputData(graph, 0.5);
    this->stlRender->axisOn();
    this->stlRender->start();
}
