//
// Created by wzx on 18-11-21.
//
#include <gtest/gtest.h>
#include "../include/Cylinder.h"
#include "../include/STLRender.h"


class CylinderTest : public ::testing::Test {

protected:
    vtkSmartPointer<Cylinder> filter;
    vtkSmartPointer<STLRender> stlRender;

    virtual void SetUp() {
        this->filter = vtkSmartPointer<Cylinder>::New();
        this->stlRender = vtkSmartPointer<STLRender>::New();
    }
};

/**
 * if you see a declining cylinder  and it is not out of shape, the test is pass
 */
TEST_F(CylinderTest, stretchTest) {
    this->stlRender->setPath("../test/res/test3.STL");
    this->stlRender->load();
    this->stlRender->axisOn();
    this->filter->setData(this->stlRender->getData());
    double vector[3] = {1.0, 1.0, 0.0};
    double basePoint[3] = {0, 0, 0};
    this->filter->stretch(2, vector, basePoint);
    this->filter->stretch(2, vector, basePoint);
    this->stlRender->setInputData(this->filter->getData());
    this->stlRender->start();
}

TEST_F(CylinderTest, hasPointTest) {
    double point1[3] = {5, 52.5, 55};
    double point2[3] = {9.95343, 51.8192, 55};
    double point3[3] = {31.799, 103.69, 0.60744};

    stlRender->setPath("../test/res/test2.STL");
    stlRender->load();
    auto cylinders = stlRender->getCylinders();

    EXPECT_TRUE(cylinders.at(0)->hasPoint(point1));
    EXPECT_TRUE(cylinders.at(0)->hasPoint(point2));
    EXPECT_FALSE(cylinders.at(0)->hasPoint(point3));

}