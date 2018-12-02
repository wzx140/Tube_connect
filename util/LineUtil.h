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
#include <vtkLine.h>
#include <vtkArcSource.h>

#include "VectorUtil.h"

using std::array;

namespace LineUtil {
    const double zAxis[3] = {0, 0, 1};
    const double xAxis[3] = {1, 0, 0};
    const double yAxis[3] = {0, 1, 0};

    /**
     * generate a curved line to blend two lines
     * @param stPoint: the start point
     * @param stVector: the vector of the start point in the start line
     * @param endPoint: the end point
     * @param endVector: the vector of the end point in the end line
     * @param resolution: the num of the points in generated line
     ** @warning if two lines is parallel, it may cause error
     * @return
     */
    inline vtkSmartPointer<vtkPolyData>
    lineBlend(array<double, 3> &stPoint, array<double, 3> &stVector, array<double, 3> &endPoint,
              array<double, 3> &endVector, int resolution);

    /**
     * get intersection of two lines in xy plane, the z-axis is 0
     * @param point1
     * @param vector1
     * @param point2
     * @param vector2
     * @warning if two lines is parallel, it may cause error
     * @return
     */
    inline array<double, 3> intersection(array<double, 3> &point1, array<double, 3> &vector1, array<double, 3> &point2,
                                         array<double, 3> &vector2);


}

namespace LineUtil {

    vtkSmartPointer<vtkPolyData>
    lineBlend(array<double, 3> &stPoint, array<double, 3> &stVector, array<double, 3> &endPoint,
              array<double, 3> &endVector, int resolution) {
        auto points = vtkSmartPointer<vtkPoints>::New();
        auto vectors = vtkSmartPointer<vtkDoubleArray>::New();
        auto data = vtkSmartPointer<vtkPolyData>::New();
        auto transform = vtkSmartPointer<vtkTransform>::New();
        auto filter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        array<double, 3> stPointZ{};
        array<double, 3> stVectorZ{};
        array<double, 3> endPointZ{};
        array<double, 3> endVectorZ{};

        points->InsertNextPoint(stPoint.data());
        points->InsertNextPoint(endPoint.data());
        data->SetPoints(points);
        vectors->SetNumberOfComponents(3);
        vectors->InsertNextTuple(stVector.data());
        vectors->InsertNextTuple(endVector.data());
        data->GetPointData()->SetVectors(vectors);


//      rotate to xy-z
        array<double, 3> nAxis = {0};
        vtkMath::Cross(stVector.data(), endVector.data(), nAxis.data());
        VectorUtil::regularize(nAxis);
        double theta = vtkMath::DegreesFromRadians(acos(vtkMath::Dot(nAxis.data(), zAxis)));
        array<double, 3> axis = {0};
        vtkMath::Cross(nAxis.data(), zAxis, axis.data());
        transform->RotateWXYZ(theta, axis.data());
        filter->SetTransform(transform);
        filter->SetInputData(data);
        filter->Update();
        filter->GetOutput()->GetPoint(0, stPointZ.data());
        filter->GetOutput()->GetPoint(1, endPointZ.data());
        filter->GetOutput()->GetPointData()->GetVectors()->GetTuple(0, stVectorZ.data());
        filter->GetOutput()->GetPointData()->GetVectors()->GetTuple(1, endVectorZ.data());

//        if (stPointZ[2] != endPointZ[2]) {
//            cout << "error" << endl;
//        }
        double pointZ = stPointZ[2];


//      get circle center
        array<double, 3> verStVector{};
        array<double, 3> verEndVector{};
        auto verStVectors = VectorUtil::getVerVector(stVectorZ);
        auto comVector = VectorUtil::getVector(stPointZ, endPointZ);
        if (VectorUtil::getAngle(comVector, verStVectors[0]) < 90) {
            verStVector = verStVectors[0];
        } else {
            verStVector = verStVectors[1];
        }

        auto verEndVectors = VectorUtil::getVerVector(endVectorZ);
        VectorUtil::reverseVector(comVector);
        if (VectorUtil::getAngle(comVector, verStVectors[0]) < 90) {
            verEndVector = verEndVectors[0];
        } else {
            verEndVector = verEndVectors[1];
        }
        auto center = LineUtil::intersection(stPointZ, verStVector, endPointZ, verEndVector);
        center[2] = pointZ;

        auto arc = vtkSmartPointer<vtkArcSource>::New();
        arc->SetCenter(center.data());
        arc->SetPoint1(stPointZ.data());
        arc->SetPoint2(endPointZ.data());
        arc->SetResolution(resolution);
        arc->Update();

//      rotate back
        filter->SetTransform(transform->GetInverse());
        filter->SetInputData(arc->GetOutput());
        filter->Update();

        return filter->GetOutput();
    }

    array<double, 3> intersection(array<double, 3> &point1, array<double, 3> &vector1, array<double, 3> &point2,
                                  array<double, 3> &vector2) {
        array<double, 3> Secpoint = {0};
        double k1 = vector1[1] / vector1[0];
        double k2 = vector2[1] / vector2[0];

        Secpoint[0] = (point2[1] - point1[1] + k1 * point1[0] - k2 * point2[0]) / (k1 - k2);
        Secpoint[1] = k1 * (Secpoint[0] - point1[0]) + point1[1];

        return Secpoint;

    }


}


#endif //TUBE_CONNECT_LINEUTIL_H
