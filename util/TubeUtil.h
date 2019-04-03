//
// Created by wzx on 18-12-3.
//

#ifndef TUBE_CONNECT_TUBEUTIL_H
#define TUBE_CONNECT_TUBEUTIL_H

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkDoubleArray.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <array>
#include <map>
#include <vector>
#include <vtkLine.h>
#include <vtkTubeFilter.h>
#include <vtkLineSource.h>

#include "LineUtil.h"
#include "CircleUtil.h"

using std::array;
using std::pair;
using std::vector;
using std::sort;
using std::greater;

namespace TubeUtil {


    inline array<array<double, 3>, 3> getEdgePoint(vector<array<double, 3>> &points, array<double, 3> &normal);

    /**
     *  generate a tube around each input line
     *  @param lines: include lots of lines
     *  @param radius
     *  @param side: side of the tube
     * @return
     */
    inline vtkSmartPointer<vtkPolyData> createTube(vector<array<array<double, 3>, 2>> &lines, double radius, int side);

    /**
     *  change the value subject to the distance of the tube
     *  @param stPoint: start center point of the tube
     *  @param endPoint: end center point of the tube
     *  @param radius
     *  @param resolution: resolution of tube points
     * @return
     */
    inline vtkSmartPointer<vtkPolyData>
    createTube(array<double, 3> &stPoint, array<double, 3> &endPoint, double radius, int resolution);

}

namespace TubeUtil {


    vtkSmartPointer<vtkPolyData> createTube(vector<array<array<double, 3>, 2>> &lines, double radius, int side) {
        auto graph = vtkSmartPointer<vtkPolyData>::New();
        auto points = vtkSmartPointer<vtkPoints>::New();
        auto lines_ = vtkSmartPointer<vtkCellArray>::New();

        for (const auto &line : lines) {
            array<double, 3> point1 = line[0];
            array<double, 3> point2 = line[1];
            auto id1 = points->InsertNextPoint(point1.data());
            auto id2 = points->InsertNextPoint(point2.data());

            auto line_ = vtkSmartPointer<vtkLine>::New();
            line_->GetPointIds()->SetId(0, id1);
            line_->GetPointIds()->SetId(1, id2);
            lines_->InsertNextCell(line_);
        }
        graph->SetPoints(points);
        graph->SetLines(lines_);
        auto filter = vtkSmartPointer<vtkTubeFilter>::New();
        filter->SetInputData(graph);
        filter->SetNumberOfSides(side);
        filter->SetRadius(radius);
        filter->Update();
        return filter->GetOutput();
    }

    vtkSmartPointer<vtkPolyData>
    createTube(array<double, 3> &stPoint, array<double, 3> &endPoint, double radius, int resolution) {

//        todo:实现类似插件功能

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


}

#endif //TUBE_CONNECT_TUBEUTIL_H
