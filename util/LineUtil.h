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
#include <vtkParametricSpline.h>
#include <vtkParametricFunctionSource.h>
#include <vtkPoints.h>
#include <vtkCleanPolyData.h>

#include "VectorUtil.h"


using std::array;
using std::vector;

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
     * @param center: the intersection point
     * @param resolution: the num of the points in generated line
     ** @warning if two lines is parallel, it may cause error
     * @return
     */
    inline vtkSmartPointer<vtkPolyData>
    lineBlend(array<double, 3> &stPoint, array<double, 3> &stVector, array<double, 3> &endPoint,
              array<double, 3> &endVector, array<double, 3> &center, int resolution);

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
     * get intersection of two line segments
     * @param point11
     * @param point12
     * @param point21
     * @param point22
     * @param point: intersection
     * @return 1->intersect 0->not intersect
     */
    inline int
    intersection3D(array<double, 3> &point11, array<double, 3> &point12, array<double, 3> &point21,
                   array<double, 3> &point22, array<double, 3> &point);

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

    /**
     * extend the line
     * @param point1
     * @param point2
     */
    inline void extend(array<double, 3> &point1, array<double, 3> &point2, double length);

    /**
     * cut the line with point and length
     * @param point1: point of the line
     * @param point2: point of the line
     * @param point: the cut point
     * @param length: the length of the cut, two-way
     * @return two lines defined by four points
     */
    inline array<array<double, 3>, 4>
    cut(array<double, 3> &point1, array<double, 3> &point2, array<double, 3> &point, double length);

    /**
     * cut the line with points and length
     * @param point1: point of the line
     * @param point2: point of the line
     * @param points: the cut points
     * @param length: the length of the each cut, two-way
     * @return if the number of the points is n, return n+1 lines defined by 2(n+1) points
     */
    inline vector<array<array<double, 3>, 2>>
    cut(array<double, 3> &point1, array<double, 3> &point2, vector<array<double, 3>> &points, vector<double> &length);

    /**
     * Are the two segments on the same line
     * @param line1
     * @param line2
     * @return
     */
    inline bool isCollinear(array<array<double, 3>, 2> &line1, array<array<double, 3>, 2> &line2);


}

namespace LineUtil {

    vtkSmartPointer<vtkPolyData>
    lineBlend(array<double, 3> &stPoint, array<double, 3> &stVector, array<double, 3> &endPoint,
              array<double, 3> &endVector, array<double, 3> &center, int resolution) {

        double distance1 = getLength(stPoint, center);
        double distance2 = getLength(endPoint, center);
        auto dPoint1 = VectorUtil::movePoint(stPoint, stVector, distance1 * 10);
        auto dPoint2 = VectorUtil::movePoint(endPoint, endVector, distance2 * 10);
        array<double, 3> intersection{0, 0, 0};
        int state = intersection3D(stPoint, dPoint1, endPoint, dPoint2, intersection);


        double length = getLength(stPoint, endPoint) / 3;
        auto stPoint2 = VectorUtil::movePoint(stPoint, stVector, length);
        auto endPoint2 = VectorUtil::movePoint(endPoint, endVector, length);

        auto points = vtkSmartPointer<vtkPoints>::New();
        points->InsertNextPoint(stPoint.data());
        points->InsertNextPoint(stPoint2.data());

        if (state == 1) {
            points->InsertNextPoint(intersection.data());
        }

        points->InsertNextPoint(endPoint2.data());
        points->InsertNextPoint(endPoint.data());


        auto spline = vtkSmartPointer<vtkParametricSpline>::New();
        spline->SetPoints(points);
        spline->ClosedOff();
        auto splineSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
        splineSource->SetParametricFunction(spline);
        splineSource->GenerateNormalsOn();
        splineSource->Update();

        return splineSource->GetOutput();
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

    void extend(array<double, 3> &point1, array<double, 3> &point2, double length) {
        auto vector = VectorUtil::getVector(point1, point2);
        VectorUtil::regularize(vector);
        for (int i = 0; i < 3; i++) {
            point2[i] = point2[i] + vector[i] * (length / 2.);
        }
        VectorUtil::reverse(vector);
        for (int i = 0; i < 3; i++) {
            point1[i] = point1[i] + vector[i] * (length / 2.);
        }

    }

    int intersection3D(array<double, 3> &point11, array<double, 3> &point12, array<double, 3> &point21,
                       array<double, 3> &point22, array<double, 3> &point) {
        double u = 0;
        double v = 0;
//        2->intersection   3->collinear or parallel  0->faceted
        int state = vtkLine::Intersection3D(point11.data(), point12.data(), point21.data(), point22.data(), u, v);
        if (state == 2) {
            auto vector = VectorUtil::getVector(point11, point12);
//            cout << u << endl;
//            cout << v << endl;
            point[0] = point11[0] + vector[0] * u;
            point[1] = point11[1] + vector[1] * u;
            point[2] = point11[2] + vector[2] * u;
            return 1;
        } else {
            return 0;
        }


    }

    array<array<double, 3>, 4>
    cut(array<double, 3> &point1, array<double, 3> &point2, array<double, 3> &point, double length) {
        array<array<double, 3>, 4> points{};
        array<double, 3> point_1{};
        array<double, 3> point_2{};

        auto vector = VectorUtil::getVector(point, point1);
        VectorUtil::regularize(vector);
        point_1.at(0) = point.at(0) + vector.at(0) * length;
        point_1.at(1) = point.at(1) + vector.at(1) * length;
        point_1.at(2) = point.at(2) + vector.at(2) * length;

        VectorUtil::reverse(vector);
        point_2.at(0) = point.at(0) + vector.at(0) * length;
        point_2.at(1) = point.at(1) + vector.at(1) * length;
        point_2.at(2) = point.at(2) + vector.at(2) * length;

        points.at(0) = point1;
        points.at(1) = point_1;
        points.at(2) = point2;
        points.at(3) = point_2;

        return points;
    }

    vector<array<array<double, 3>, 2>>
    cut(array<double, 3> &point1, array<double, 3> &point2, vector<array<double, 3>> &points, vector<double> &length) {
        //FIXME: may have some bug
        vector<array<array<double, 3>, 2>> returnLines;

        // sort
        auto vector = VectorUtil::getVector(point1, point2);
        for (int i = 0; i < points.size(); i++) {
            for (int j = i; j < points.size(); j++) {
                auto length1 = LineUtil::getLength(point1, points.at(i));
                auto length2 = LineUtil::getLength(point1, points.at(j));
                if (length1 > length2) {
                    std::swap(points.at(i), points.at(j));
                    std::swap(length.at(i), length.at(j));
                }
            }
        }

        // cut
        array<array<double, 3>, 2> line2{};
        line2.at(0) = point1;
        line2.at(1) = point2;
        for (int i = 0; i < points.size(); i++) {
            auto lines = cut(line2.at(0), line2.at(1), points.at(i), length.at(i));
            array<array<double, 3>, 2> line1{};
            line1.at(0) = lines[0];
            line1.at(1) = lines[1];
            line2.at(0) = lines[3];
            line2.at(1) = lines[2];
            returnLines.emplace_back(line1);
        }
        returnLines.emplace_back(line2);

        return returnLines;

    }

    bool isCollinear(array<array<double, 3>, 2> &line1, array<array<double, 3>, 2> &line2) {
        auto vector1 = VectorUtil::getVector(line1[0], line1[1]);
        auto vector2 = VectorUtil::getVector(line2[0], line2[1]);
        auto vector3 = VectorUtil::getVector(line1[0], line2[0]);
        VectorUtil::regularize(vector1);
        VectorUtil::regularize(vector2);
        VectorUtil::regularize(vector3);

        if (!VectorUtil::isEqual(vector1, vector2)) {
            VectorUtil::reverse(vector2);
        }

        if (!VectorUtil::isEqual(vector1, vector3)) {
            VectorUtil::reverse(vector3);
        }

        return VectorUtil::isEqual(vector1, vector2) && VectorUtil::isEqual(vector1, vector3);

    }


}


#endif //TUBE_CONNECT_LINEUTIL_H
