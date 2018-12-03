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


}

namespace TubeUtil {

    bool cmp(const pair<array<array<double, 3>, 2>, double> &p1, const pair<array<array<double, 3>, 2>, double> &p2) {
        return p1.second < p2.second;
    }

    vector<vtkSmartPointer<vtkPolyData>>
    connect(vtkSmartPointer<vtkDoubleArray> edge1, array<double, 3> vector1, vtkSmartPointer<vtkDoubleArray> edge2,
            array<double, 3> vector2, double proportion, int resolution) {


        vector<vtkSmartPointer<vtkPolyData>> data{};
        vector<pair<array<array<double, 3>, 2>, double>> distances;
        pair<array<array<double, 3>, 2>, double> distance;
        array<double, 3> point1{};
        array<double, 3> point2{};
        for (int i = 0; i < edge1->GetNumberOfTuples(); i++) {
            edge1->GetTuple(i, point1.data());
            for (int j = 0; j < edge1->GetNumberOfTuples(); j++) {
                edge2->GetTuple(j, point2.data());
                double distanceTemp = sqrt(
                        pow(point1[0] - point2[0], 2) + pow(point1[1] - point2[1], 2) + pow(point1[2] - point2[2], 2));
                if (distanceTemp < distance.second || j == 0) {
                    distance.first = array<array<double, 3>, 2>{point1, point2};
                    distance.second = distanceTemp;
                }
            }
            distances.push_back(distance);
        }
        sort(distances.begin(), distances.end(), cmp);
//        cout << distances.size() * proportion << endl;
        for (int i = 0; i < distances.size() * proportion; i++) {
            auto lineData = LineUtil::lineBlend(distances[i].first[0], vector1, distances[i].first[1], vector2,
                                                resolution);
//            cout << lineData->GetNumberOfPoints()<<endl;
            data.push_back(lineData);
        }
        return data;
    }

}

#endif //TUBE_CONNECT_TUBEUTIL_H
