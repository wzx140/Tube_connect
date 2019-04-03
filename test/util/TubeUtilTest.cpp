//
// Created by wzx on 19-3-30.
//

#include <gtest/gtest.h>
#include <vtkDoubleArray.h>
#include <vtkCellArray.h>

#include "../../util/TubeUtil.h"
#include "../../include/STLRender.h"

using namespace TubeUtil;

TEST(TubeUtilTest, createTubeTest) {
    array<double, 3> stPoint{10, 10, 10};
    array<double, 3> endPoint{-1, -1, -1};
//    todo:测试距离函数
    auto data = createTube(stPoint, endPoint, 5, 50);
//    auto stlRender = vtkSmartPointer<STLRender>::New();
//    stlRender->setInputData(data, 1);
//    stlRender->axisOn();
//    stlRender->start();
}