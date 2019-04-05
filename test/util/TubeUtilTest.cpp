//
// Created by wzx on 19-3-30.
//

#include <gtest/gtest.h>
#include <vtkDoubleArray.h>
#include <vtkCellArray.h>
#include <vtkSampleFunction.h>
#include <vtkContourFilter.h>

#include "../../util/TubeUtil.h"
#include "../../include/STLRender.h"

using namespace TubeUtil;

TEST(TubeUtilTest, createTubeTest) {
    array<double, 3> stPoint{0, -5, 0};
    array<double, 3> endPoint{0, -1, 0};
    auto data = createTube(stPoint, endPoint, 5);

    vtkSmartPointer<vtkSampleFunction> theConeSample =
            vtkSmartPointer<vtkSampleFunction>::New();
    theConeSample->SetImplicitFunction(data);
    theConeSample->SetModelBounds(-40, 40, -40, 40, -40, 40);
    theConeSample->SetSampleDimensions(128, 128, 128);
    theConeSample->ComputeNormalsOff();

    vtkSmartPointer<vtkContourFilter> theConeSurface =
            vtkSmartPointer<vtkContourFilter>::New();
    theConeSurface->SetInputConnection(theConeSample->GetOutputPort());
    theConeSurface->SetValue(0, 0.0);
    theConeSurface->Update();

    auto stlRender = vtkSmartPointer<STLRender>::New();
    stlRender->setInputData(theConeSurface->GetOutput(), 1);
    stlRender->axisOn();
    stlRender->start();
}

TEST(TubeUtilTest, ClipTest) {
    array<double, 3> stPoint{0, -5, 0};
    array<double, 3> endPoint{0, -1, 0};
    array<double, 3> stPoint1{0, -5.2, 0};
    array<double, 3> endPoint1{0, -4.8, 0};
    array<double, 3> stPoint2{0, -0.8, 0};
    array<double, 3> endPoint2{0, -1.2, 0};
    auto data = createTube(stPoint, endPoint, 5);
    auto clip1 = createTube(stPoint1, endPoint1, 5.2);
    auto clip2 = createTube(stPoint2, endPoint2, 5.2);
    vector<vtkSmartPointer<vtkImplicitFunction>> clips;
    clips.emplace_back(clip1);
    clips.emplace_back(clip2);

    vtkSmartPointer<vtkSampleFunction> theConeSample =
            vtkSmartPointer<vtkSampleFunction>::New();
    theConeSample->SetImplicitFunction(data);
    theConeSample->SetModelBounds(-40, 40, -40, 40, -40, 40);
    theConeSample->SetSampleDimensions(128, 128, 128);
    theConeSample->ComputeNormalsOff();

    vtkSmartPointer<vtkContourFilter> theConeSurface =
            vtkSmartPointer<vtkContourFilter>::New();
    theConeSurface->SetInputConnection(theConeSample->GetOutputPort());
    theConeSurface->SetValue(0, 0.0);
    theConeSurface->Update();

    auto data2 = clip(theConeSurface->GetOutput(), clips);
    auto stlRender = vtkSmartPointer<STLRender>::New();
    stlRender->setInputData(data2, 1);
    stlRender->axisOn();
    stlRender->start();
}
