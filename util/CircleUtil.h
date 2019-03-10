//
// Created by wzx on 18-12-4.
//

#ifndef TUBE_CONNECT_CIRCLEUTIL_H
#define TUBE_CONNECT_CIRCLEUTIL_H

#include <array>
#include <vector>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkPolyData.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPoints.h>

#include "VectorUtil.h"
#include "LineUtil.h"
#include "VectorUtil.h"

using std::array;
using std::vector;


namespace CircleUtil {

    /**
     * get the center of the circle which is defined by three points
     * @param point1
     * @param point2
     * @param point3
     * @param vector: the normal of the circle
     * @return
     */
    inline array<double, 3>
    getCenter(array<double, 3> &point1, array<double, 3> &point2, array<double, 3> &point3, array<double, 3> &vector);

}

namespace CircleUtil {
    array<double, 3>
    getCenter(array<double, 3> &point1, array<double, 3> &point2, array<double, 3> &point3, array<double, 3> &vector) {
        auto points = vtkSmartPointer<vtkPoints>::New();
        auto data = vtkSmartPointer<vtkPolyData>::New();
        auto transform = vtkSmartPointer<vtkTransform>::New();
        auto filter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        array<double, 3> point1Z{};
        array<double, 3> point2Z{};
        array<double, 3> point3Z{};

        points->InsertNextPoint(point1.data());
        points->InsertNextPoint(point2.data());
        points->InsertNextPoint(point3.data());
        data->SetPoints(points);

//      rotate to xy-z
        VectorUtil::regularize(vector);
        double theta = vtkMath::DegreesFromRadians(acos(vtkMath::Dot(vector.data(), LineUtil::zAxis)));
        array<double, 3> axis = {0};
        vtkMath::Cross(vector.data(), LineUtil::zAxis, axis.data());
        transform->RotateWXYZ(theta, axis.data());
        filter->SetTransform(transform);
        filter->SetInputData(data);
        filter->Update();
        filter->GetOutput()->GetPoint(0, point1Z.data());
        filter->GetOutput()->GetPoint(1, point2Z.data());
        filter->GetOutput()->GetPoint(2, point3Z.data());

        points->Reset();
//        cout << points->GetNumberOfPoints()<<endl;

        double pointZ = point1Z[2];
        auto center1 = LineUtil::getCenter(point1Z, point2Z);
        auto vector1 = VectorUtil::getVector(point1Z, point2Z);
        auto center2 = LineUtil::getCenter(point2Z, point3Z);
        auto vector2 = VectorUtil::getVector(point2Z, point3Z);


//      get circle interPoint
        auto verVectors1 = VectorUtil::getVerVector(vector1);
        auto verVectors2 = VectorUtil::getVerVector(vector2);

        auto interPoint = LineUtil::intersection(center1, verVectors1[0], center2, verVectors2[0]);
        interPoint[2] = pointZ;

        points->InsertNextPoint(interPoint.data());

        data->Reset();
        data->SetPoints(points);

//      rotate back
        filter->SetTransform(transform->GetInverse());
        filter->SetInputData(data);
        filter->Update();

        array<double, 3> center = {0, 0, 0};
        filter->GetOutput()->GetPoints()->GetPoint(0, center.data());


        return center;
    }
}

#endif //TUBE_CONNECT_CIRCLEUTIL_H
