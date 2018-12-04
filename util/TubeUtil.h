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

#include "LineUtil.h"

using std::array;
using std::pair;
using std::vector;

namespace TubeUtil {

    /**
     * generate some lines to connect two edge of tube
     * @param edge1
     * @param vector1: the normal of the edge1
     * @param edge2
     * @param vector2: the normal of the edge2
     * @param proportion: the proportion of the connected edge
     * @param resolution: the resolution of sampling
     * @warning the number of points in edge1 must be equal to edge2
     * @return some points in the lines
     */
    inline vector<vtkSmartPointer<vtkPolyData>>
    connect(vtkSmartPointer<vtkDoubleArray> edge1, array<double, 3> vector1, vtkSmartPointer<vtkDoubleArray> edge2,
            array<double, 3> vector2, double proportion, int resolution);

    /**
     * get the start point pair between two edge. In other words, it is the point pair in the shortest distance
     * @param edge1
     * @param edge2
     * @return the index of two points
     */
    inline array<int, 2>
    getStPointPair(const vtkSmartPointer<vtkDoubleArray> edge1, const vtkSmartPointer<vtkDoubleArray> edge2);

    /**
     * get the next index, like circulation
     * @param index: the previous index
     * @param num: the num of the index
     * @param mode: 0->index+1, 1->index-1
     * @return
     */
    inline int next(int index, int num, int mode);

}

namespace TubeUtil {

    vector<vtkSmartPointer<vtkPolyData>>
    connect(vtkSmartPointer<vtkDoubleArray> edge1, array<double, 3> vector1, vtkSmartPointer<vtkDoubleArray> edge2,
            array<double, 3> vector2, double proportion, int resolution) {

        vector<vtkSmartPointer<vtkPolyData>> data;
        auto index = getStPointPair(edge1, edge2);
        int num = static_cast<int>(edge1->GetNumberOfTuples() * proportion / 2);
        int index1 = index[0];
        int index2 = index[1];
        int index1Temp = index1;
        int index2Temp = index2;
        array<double, 3> point1{};
        array<double, 3> point2{};
        int num1 = edge1->GetNumberOfTuples();

        edge1->GetTuple(index1, point1.data());
        edge2->GetTuple(index2, point2.data());
        auto lineData = LineUtil::lineBlend(point1, vector1, point2, vector2, resolution);

        data.push_back(lineData);

//      determine whether it is a positive point pair or a reverse point pair
        int mode = 0;
        edge1->GetTuple(next(index1, num1, 0), point1.data());
        edge2->GetTuple(next(index2, num1, 0), point2.data());
        double distance1 = LineUtil::getLength(point1, point2);

        edge1->GetTuple(next(index1, num1, 0), point1.data());
        edge2->GetTuple(next(index2, num1, 1), point2.data());
        double distance2 = LineUtil::getLength(point1, point2);

        if (distance1 < distance2) {
            mode = 0;
        } else {
            mode = 1;
        }

//        generate lines
        if (mode == 0) {
            for (int i = 0; i < num; i++) {
                index1Temp = next(index1Temp, num1, 0);
                index2Temp = next(index2Temp, num1, 0);
                edge1->GetTuple(index1Temp, point1.data());
                edge2->GetTuple(index2Temp, point2.data());
                lineData = LineUtil::lineBlend(point1, vector1, point2, vector2, resolution);
                data.push_back(lineData);
            }
            index1Temp = index1;
            index2Temp = index2;
            for (int i = 0; i < num; i++) {
                index1Temp = next(index1Temp, num1, 1);
                index2Temp = next(index2Temp, num1, 1);
                edge1->GetTuple(index1Temp, point1.data());
                edge2->GetTuple(index2Temp, point2.data());
                lineData = LineUtil::lineBlend(point1, vector1, point2, vector2, resolution);
                data.push_back(lineData);
            }

        } else {
            for (int i = 0; i < num; i++) {
                index1Temp = next(index1Temp, num1, 1);
                index2Temp = next(index2Temp, num1, 0);
                edge1->GetTuple(index1Temp, point1.data());
                edge2->GetTuple(index2Temp, point2.data());
                lineData = LineUtil::lineBlend(point1, vector1, point2, vector2, resolution);
                data.push_back(lineData);
            }
            index1Temp = index1;
            index2Temp = index2;
            for (int i = 0; i < num; i++) {
                index1Temp = next(index1Temp, num1, 0);
                index2Temp = next(index2Temp, num1, 1);
                edge1->GetTuple(index1Temp, point1.data());
                edge2->GetTuple(index2Temp, point2.data());
                lineData = LineUtil::lineBlend(point1, vector1, point2, vector2, resolution);
                data.push_back(lineData);
            }
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
    getStPointPair(const vtkSmartPointer<vtkDoubleArray> edge1, const vtkSmartPointer<vtkDoubleArray> edge2) {
        double distance = DBL_MAX;
        array<int, 2> index{};
        array<double, 3> point1{};
        array<double, 3> point2{};

        for (int i = 0; i < edge1->GetNumberOfTuples(); i++) {
            edge1->GetTuple(i, point1.data());
            for (int j = 0; j < edge2->GetNumberOfTuples(); j++) {
                edge2->GetTuple(j, point2.data());
                double length = LineUtil::getLength(point1, point2);
                if (length < distance) {
                    distance = length;
                    index[0] = i;
                    index[1] = j;
                }
            }
        }
        return index;
    }

}

#endif //TUBE_CONNECT_TUBEUTIL_H
