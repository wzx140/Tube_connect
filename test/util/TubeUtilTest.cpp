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

    std::array<double, 3> point1{0.0011373422409252966,10.995489073403645,200.00067138671875};
    std::array<double, 3> point2{-0.0011383570664462508,-11.005300106991923,200.00067138671875};

    std::array<double, 3> point3{10.999997407207733,-0.0049055167896668818,199.99320093381806};
    std::array<double, 3> point4{-10.999998422033254,-0.0049055167986110692,200.00814183961944};

    std::array<double, 3> point5{7.7785456822478931,7.7734537180288852,199.99526545353299};
    std::array<double, 3> point6{-7.7785466970734145,-7.7832647516171631,200.00607731990451};


    auto c1 = TubeUtil::createTube(point1, point2, 3, 30);
    auto c2 = TubeUtil::createTube(point3, point4, 3, 30);
    auto c3 = TubeUtil::createTube(point5, point6, 3, 30);

    vector<vtkSmartPointer<vtkPolyData>> data;
    data.emplace_back(c1);
    data.emplace_back(c2);
    data.emplace_back(c3);

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
//
//
//    auto booleanFilter = vtkSmartPointer<vtkPolyDataBooleanFilter>::New();
//    booleanFilter->SetInputData(0, c1);
//    booleanFilter->SetInputData(1, c4);
//    booleanFilter->Update();

    this->stlRender->setInputData(data, 1);
    this->stlRender->axisOn();
    this->stlRender->start();
}
