//
// Created by wzx on 18-12-1.
//

#ifndef TUBE_CONNECT_LINEUTIL_H
#define TUBE_CONNECT_LINEUTIL_H

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkMath.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <array>

#include "VectorUtil.h"

using std::array;

namespace LineUtil {
    const double zAxis[3] = {0, 0, 1};

    /**
     * generate a curved line to blend two lines
     * @param stPoint: the start point
     * @param stVector: the vector of the start point in the start line
     * @param endPoint: the end point
     * @param endVector: the vector of the end point in the end line
     * @param resolution: the num of the points in generated line
     * @return
     */
    vtkSmartPointer<vtkPoints>
    lineBlend(double stPoint[], double stVector[], double endPoint[], double endVector[], int resolution);

    array<array<double, 3>, 2> getLine(double st)


}

namespace LineUtil {

    vtkSmartPointer<vtkPoints>
    lineBlend(double *stPoint, double *stVector, double *endPoint, double *endVector, int resolution) {
        auto points = vtkSmartPointer<vtkPoints>::New();
        auto vectors = vtkSmartPointer<vtkDoubleArray>::New();
        auto data = vtkSmartPointer<vtkPolyData>::New();
        auto transform = vtkSmartPointer<vtkTransform>::New();
        auto filter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        double stPointZ[3];
        double endPointZ[3];

        points->InsertNextPoint(stPoint);
        points->InsertNextPoint(endPoint);
        data->SetPoints(points);
        vectors->SetNumberOfComponents(3);
        vectors->InsertNextTuple(stVector);
        vectors->InsertNextTuple(endVector);
        data->GetPointData()->SetVectors(vectors);

//      rotate to xy-z
        double nAxis[3];
        vtkMath::Cross(stVector, endVector, nAxis);
        VectorUtil::regularize(nAxis);
        auto theta = vtkMath::DegreesFromRadians(acos(vtkMath::Dot(nAxis, zAxis)));
        double axis[3];
        vtkMath::Cross(nAxis, zAxis, axis);
        transform->RotateWXYZ(theta, axis);
        filter->SetTransform(transform);
        filter->Update();
        filter->GetOutput()->GetPoint(0, stPointZ);
        filter->GetOutput()->GetPoint(1, endPointZ);

        if (stPointZ[2] != endPointZ[2]) {
            cout << "error" << endl;
        }


        return NULL;
    }
}


#endif //TUBE_CONNECT_LINEUTIL_H
