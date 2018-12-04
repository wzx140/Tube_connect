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
#include <vtkLine.h>
#include <vtkArcSource.h>
#include <array>

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

    /**
     * get the center point of the line
     * @param stPoint: the start point in the line
     * @param endPoint: the end point in the line
     * @return
     */
    inline array<double, 3> getCenter(array<double, 3> &stPoint, array<double, 3> &endPoint);

    /**
     * get length of two points
     * @param stPoint
     * @param endPoint
     * @return
     */
    inline double getLength(array<double, 3> &stPoint, array<double, 3> &endPoint);


}

namespace LineUtil {

    vtkSmartPointer<vtkPolyData>
    lineBlend(array<double, 3> &stPoint, array<double, 3> &stVector, array<double, 3> &endPoint,
              array<double, 3> &endVector, int resolution) {
        auto points = vtkSmartPointer<vtkPoints>::New();
        auto vectors = vtkSmartPointer<vtkDoubleArray>::New();
        auto data = vtkSmartPointer<vtkPolyData>::New();

//        straight line
//        auto vector = VectorUtil::getVector(stPoint, endPoint);
//        vector[0] = vector[0] / resolution;
//        vector[1] = vector[1] / resolution;
//        vector[2] = vector[2] / resolution;
//        for (int i = 0; i < resolution; i++) {
//            array<double, 3> point{};
//            point[0] = stPoint[0] + vector[0]*(i+1);
//            point[1] = stPoint[1] + vector[1]*(i+1);
//            point[2] = stPoint[2] + vector[2]*(i+1);
//            points->InsertNextPoint(point.data());
//        }
//        data->SetPoints(points);
//        return data;


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
        auto verStVectors = VectorUtil::getVerVector(stVectorZ);
        auto verEndVectors = VectorUtil::getVerVector(endVectorZ);
        auto center = LineUtil::intersection(stPointZ, verStVectors[0], endPointZ, verEndVectors[0]);
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
        array<double, 3> SecPoint = {0};

        double k1 = vector1[1] / vector1[0];
        double k2 = vector2[1] / vector2[0];

        if (vector1[0] == 0) {
            SecPoint[0] = point1[0];
            SecPoint[1] = k2 * (SecPoint[0] - point2[0]) + point2[1];
            return SecPoint;

        } else if (vector2[0] == 0) {
            SecPoint[0] = point2[0];
            SecPoint[1] = k1 * (SecPoint[0] - point1[0]) + point1[1];
            return SecPoint;
        }

        SecPoint[0] = (point2[1] - point1[1] + k1 * point1[0] - k2 * point2[0]) / (k1 - k2);
        SecPoint[1] = k1 * (SecPoint[0] - point1[0]) + point1[1];

        return SecPoint;

    }

    array<double, 3> getCenter(array<double, 3> &stPoint, array<double, 3> &endPoint) {
        array<double, 3> center{};
        center[0] = (stPoint[0] + endPoint[0]) / 2;
        center[1] = (stPoint[1] + endPoint[1]) / 2;
        center[2] = (stPoint[2] + endPoint[2]) / 2;
        return center;
    }

    double getLength(array<double, 3> &stPoint, array<double, 3> &endPoint) {
        return sqrt(pow(stPoint[0] - endPoint[0], 2) + pow(stPoint[1] - endPoint[1], 2) +
                    pow(stPoint[2] - endPoint[2], 2));
    }


}


#endif //TUBE_CONNECT_LINEUTIL_H
