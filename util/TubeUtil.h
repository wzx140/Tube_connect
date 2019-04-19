//
// Created by wzx on 18-12-3.
//

#ifndef TUBE_CONNECT_TUBEUTIL_H
#define TUBE_CONNECT_TUBEUTIL_H

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <array>
#include <vector>
#include <vtkTubeFilter.h>
#include <vtkLineSource.h>
#include <vtkCylinder.h>
#include <vtkImplicitBoolean.h>
#include <vtkImplicitFunction.h>
#include <vtkPlane.h>
#include <vtkClipPolyData.h>
#include <vtkCylinderSource.h>
#include <vtkImplicitPolyDataDistance.h>

#include "../include/STLRender.h"
#include "../include/Tube.h"
#include "LineUtil.h"
#include "CircleUtil.h"

using std::array;
using std::vector;

namespace TubeUtil {


    inline array<array<double, 3>, 3> getEdgePoint(vector<array<double, 3>> &points, array<double, 3> &normal);

    /**
     *  generate the implicit surface of the tube
     *  @param stPoint: start center point of the tube
     *  @param endPoint: end center point of the tube
     *  @param radius
     * @return
     */
    inline vtkSmartPointer<vtkImplicitFunction>
    createTube(array<double, 3> &stPoint, array<double, 3> &endPoint, double radius);

    /**
     * remove the region of tubes out of data
     * @param data
     * @param tubes: the region to remove
     * @return
     */
    inline vtkSmartPointer<vtkPolyData>
    clip(vtkSmartPointer<vtkPolyData> data, vector<vtkSmartPointer<vtkImplicitFunction>> tubes);
}

namespace TubeUtil {

    vtkSmartPointer<vtkImplicitFunction>
    createTube(array<double, 3> &stPoint, array<double, 3> &endPoint, double radius) {
        auto center = LineUtil::getCenter(stPoint, endPoint);
        auto normal = VectorUtil::getVector(stPoint, endPoint);

        auto plane1 = vtkSmartPointer<vtkPlane>::New();
        plane1->SetOrigin(stPoint.data());
        VectorUtil::reverse(normal);
        plane1->SetNormal(normal.data());
        VectorUtil::reverse(normal);
        auto plane2 = vtkSmartPointer<vtkPlane>::New();
        plane2->SetOrigin(endPoint.data());
        plane2->SetNormal(normal.data());

        auto cylinder = vtkSmartPointer<vtkCylinder>::New();
        cylinder->SetRadius(radius);
        cylinder->SetCenter(center.data());
        cylinder->SetAxis(normal.data());
        auto theCylinder = vtkSmartPointer<vtkImplicitBoolean>::New();
        theCylinder->SetOperationTypeToIntersection();
        theCylinder->AddFunction(cylinder);
        theCylinder->AddFunction(plane1);
        theCylinder->AddFunction(plane2);

        return theCylinder;

    }

    array<array<double, 3>, 3> getEdgePoint(vector<array<double, 3>> &points, array<double, 3> &normal) {
        int pointID = 0;
        array<double, 3> edgePoint1 = {0};
        array<double, 3> edgePoint2 = {0};
        array<double, 3> edgePoint3 = {0};

        for (int i = 0; i < points.size(); i++) {

            bool flag = true;
            edgePoint1 = points[i];

            for (int j = 0; j < points.size(); j++) {
                if (i != j) {
                    auto pointTemp = points[j];
                    auto vector = VectorUtil::getVector(edgePoint1, pointTemp);
                    double angle = VectorUtil::getAngle(vector, normal);
                    if (angle < 90 && angle > 0) {
                        flag = false;
                        break;
                    }
                }
            }
            if (flag) {
                pointID = i;
                break;
            }
        }

        //  get three point in the edge
        int pointNum = 2;
        for (int i = 0; i < points.size(); i++) {
            if (i != pointID) {
                auto point = points[i];
                auto vector = VectorUtil::getVector(point, edgePoint1);
                if (abs(VectorUtil::getAngle(vector, normal) - 90) < 0.01) {
                    if (pointNum == 2) {
                        edgePoint2 = point;
                        pointNum--;
                    } else if (pointNum == 1) {
                        edgePoint3 = point;
                        pointNum--;
                    } else if (pointNum == 0) {
                        break;
                    }
                }
            }
        }

        array<array<double, 3>, 3> edgePoints{};
        edgePoints[0] = edgePoint1;
        edgePoints[1] = edgePoint2;
        edgePoints[2] = edgePoint3;
        return edgePoints;

    }

    vtkSmartPointer<vtkPolyData>
    clip(vtkSmartPointer<vtkPolyData> data, vector<vtkSmartPointer<vtkImplicitFunction>> tubes) {
        vtkSmartPointer<vtkPolyData> result = data;
        for (auto &tube : tubes) {
            auto clipper = vtkSmartPointer<vtkClipPolyData>::New();
            clipper->SetClipFunction(tube);
            clipper->SetInputData(result);
            clipper->InsideOutOff();
            clipper->Update();
            result = clipper->GetOutput();
        }
        return result;
    }


}

#endif //TUBE_CONNECT_TUBEUTIL_H
