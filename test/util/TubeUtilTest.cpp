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


TEST_F(TubeUtilTest, connetTest) {

    std::array<double, 3> point1{-5.5622595813616411e-05,10.999984327516209,-3.4986969694728032e-05};
    std::array<double, 3> point2{5.562294964431596e-05,-11.000054301455599,-3.4986969694728032e-05};

    std::array<double, 3> point3{11.000000000081791,-3.4986969694728032e-05,-8.82771801111915e-05};
    std::array<double, 3> point4{-10.999999999727962,-3.4986969694728032e-05,1.8303240721735443e-05};

    std::array<double, 3> point5{7.7781793107273405,7.7781348878897996,-0.00014222796503328742};
    std::array<double, 3> point6{-7.7781793103735097,-7.778204861829189,7.225402564383134e-05};

    std::array<double, 3> point7{-7.778192967842096,7.7781485453203816,7.6493464694548996e-05};
    std::array<double, 3> point8{7.7781929681959268,-7.778218519259771,-0.00014646740408400506};

    auto c1 = TubeUtil::createTube(point1, point2, 3, 40);
    auto c2 = TubeUtil::createTube(point3, point4, 3, 40);
    auto c3 = TubeUtil::createTube(point5, point6, 3, 40);
    auto c4 = TubeUtil::createTube(point7, point8, 3, 40);

//    double c[] = {
//            point3[0]+(point4[0]-point4[0])/2,
//            point3[1]+(point4[1]-point4[1])/2,
//            point3[2]+(point4[2]-point4[2])/2,
//    };
//
//    auto tr = vtkSmartPointer<vtkTransform>::New();
//    tr->PostMultiply();
//    tr->Translate(-c[0], -c[1], -c[2]);
//    tr->RotateWXYZ(6, point4[0]-point3[0], point4[1]-point3[1], point4[2]-point3[2]);
//    tr->Translate(c);
//
//    auto tf = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
//    tf->SetInputData(c2);
//    tf->SetTransform(tr);
//    tf->Update();


    auto booleanFilter = vtkSmartPointer<vtkPolyDataBooleanFilter>::New();
    booleanFilter->SetInputData(0, c1);
    booleanFilter->SetInputData(1, c4);
    booleanFilter->Update();

    this->stlRender->setInputData(booleanFilter->GetOutput(), 1);
    this->stlRender->axisOn();
    this->stlRender->start();
}
