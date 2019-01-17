//
// Created by wzx on 18-12-3.
//

#ifndef TUBE_CONNECT_TUBEUTIL_H
#define TUBE_CONNECT_TUBEUTIL_H

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkDoubleArray.h>
#include <array>
#include <map>
#include <vector>
#include <vtkLine.h>
#include <vtkTubeFilter.h>

#include "LineUtil.h"

using std::array;
using std::pair;
using std::vector;
using std::sort;
using std::greater;

namespace TubeUtil {

    /**
     * generate some lines to connect two edge of tube
     * @param edge1
     * @param vector1: the normal of the edge1
     * @param edge2
     * @param vector2: the normal of the edge2
     * @param num: the number of the connected edge point
     * @param resolution: the resolution of sampling
     * @warning the number of points in edge1 must be equal to edge2, the point connected will be inactivated
     * @return some points in the lines
     */
    inline vector<vtkSmartPointer<vtkPolyData>>
    connect(vector<array<double, 3>> &edge1, array<double, 3> &vector1, vector<array<double, 3>> &edge2,
            array<double, 3> &vector2, int num, int resolution);

    inline array<array<double, 3>, 3> getEdgePoint(vector<array<double, 3>> &points, array<double, 3> &normal);

    /**
     * get the start point pair between two edge. In other words, it is the point pair in the shortest distance
     * @param edge1
     * @param edge2
     * @return the index of two points
     */
    inline array<int, 2>
    getShortPointPair(vector<array<double, 3>> &edge1, vector<array<double, 3>> &edge2);

    /**
     * get the next index, like circulation
     * @param index: the previous index
     * @param num: the num of the index
     * @param mode: 0->index+1, 1->index-1
     * @return
     */
    inline int next(int index, int num, int mode);

    /**
     *  generate a tube around each input line
     *  @param data: include lots of lines
     *  @param radius
     *  @param side: side of the tube
     * @return
     */
    inline vtkSmartPointer<vtkPolyData> createTube(vector<array<array<double, 3>, 2>> &lines, double radius, int side);

}

namespace TubeUtil {

    vector<vtkSmartPointer<vtkPolyData>>
    connect(vector<array<double, 3>> &edge1, array<double, 3> &vector1, vector<array<double, 3>> &edge2,
            array<double, 3> &vector2, int num, int resolution) {

        vector<vtkSmartPointer<vtkPolyData>> data;
        auto index = getShortPointPair(edge1, edge2);
        int index1Temp = index[0];
        int index2Temp = index[1];
        int num1 = edge1.size();
        int num2 = edge2.size();

//        the point to extinct
        vector<int> exIndex1;
        vector<int> exIndex2;
        exIndex1.push_back(index1Temp);
        exIndex2.push_back(index2Temp);

        auto lineData = LineUtil::lineBlend(edge1.at(index[0]), vector1, edge2.at(index[1]), vector2, resolution);

        data.push_back(lineData);

//      determine whether it is a positive point pair or a reverse point pair
        int mode = 0;
        double distance1 = LineUtil::getLength(edge1.at(next(index[0], num1, 0)), edge2.at(next(index[1], num2, 0)));

        double distance2 = LineUtil::getLength(edge1.at(next(index[0], num1, 0)), edge2.at(next(index[1], num2, 1)));

        if (distance1 < distance2) {
            mode = 0;
        } else {
            mode = 1;
        }

//        generate lines
        if (mode == 0) {
            for (int i = 0; i < num; i++) {
                index1Temp = next(index1Temp, num1, 0);
                index2Temp = next(index2Temp, num2, 0);
                exIndex1.push_back(index1Temp);
                exIndex2.push_back(index2Temp);
                lineData = LineUtil::lineBlend(edge1[index1Temp], vector1, edge2[index2Temp], vector2, resolution);
                data.push_back(lineData);
            }
            index1Temp = index[0];
            index2Temp = index[1];
            for (int i = 0; i < num; i++) {
                index1Temp = next(index1Temp, num1, 1);
                index2Temp = next(index2Temp, num2, 1);
                exIndex1.push_back(index1Temp);
                exIndex2.push_back(index2Temp);
                lineData = LineUtil::lineBlend(edge1[index1Temp], vector1, edge2[index2Temp], vector2, resolution);
                data.push_back(lineData);
            }

        } else {
            for (int i = 0; i < num; i++) {
                index1Temp = next(index1Temp, num1, 1);
                index2Temp = next(index2Temp, num2, 0);
                exIndex1.push_back(index1Temp);
                exIndex2.push_back(index2Temp);
                lineData = LineUtil::lineBlend(edge1[index1Temp], vector1, edge2[index2Temp], vector2, resolution);
                data.push_back(lineData);
            }
            index1Temp = index[0];
            index2Temp = index[1];
            for (int i = 0; i < num; i++) {
                index1Temp = next(index1Temp, num1, 0);
                index2Temp = next(index2Temp, num2, 1);
                exIndex1.push_back(index1Temp);
                exIndex2.push_back(index2Temp);
                lineData = LineUtil::lineBlend(edge1[index1Temp], vector1, edge2[index2Temp], vector2, resolution);
                data.push_back(lineData);
            }
        }

        sort(exIndex1.begin(), exIndex1.end(), greater<int>());
        sort(exIndex2.begin(), exIndex2.end(), greater<int>());

//     inactivated points
        for (int i = 0; i < exIndex1.size(); i++) {
            edge1.erase(edge1.begin() + exIndex1.at(i));
        }
        for (int i = 0; i < exIndex2.size(); i++) {
            edge2.erase(edge2.begin() + exIndex2.at(i));
        }

        return data;
    }

    int next(int index, int num, int mode) {
        if (mode == 0) {
            if (index == num - 1) {
                return 0;
            } else {
                return index + 1;
            }
        } else {
            if (index == 0) {
                return num - 1;
            } else {
                return index - 1;
            }
        }
    }

    array<int, 2>
    getShortPointPair(vector<array<double, 3>> &edge1, vector<array<double, 3>> &edge2) {
        double distance = DBL_MAX;
        array<int, 2> index{};

        for (int i = 0; i < edge1.size(); i++) {
            for (int j = 0; j < edge2.size(); j++) {
                double length = LineUtil::getLength(edge1[i], edge2[j]);
                if (length < distance) {
                    distance = length;
                    index[0] = i;
                    index[1] = j;
                }
            }
        }
        return index;
    }

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
