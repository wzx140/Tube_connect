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