//
// Created by wzx on 19-1-11.
//

#ifndef TUBE_CONNECT_GRAPH_H
#define TUBE_CONNECT_GRAPH_H

#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <vector>
#include <array>

#include "Intersection.h"
#include "Tube.h"

using std::vector;
using std::array;

/**
 * Overall structural lines and intersections
 */
class Graph : public vtkObject {
private:
    /**
     * Unlike other tubes, this only saves both ends points and radius
     */
    vector<vtkSmartPointer<Tube>> tubeLines;

    /**
     * intersections in graph
     */
    vector<vtkSmartPointer<Intersection>> intersections;

    /**
     * the length to extend
     */
    double length;

    /**
     * determine the room in intersections
     */
    double coefficient1;

    /**
     * determine the sample in intersection
     */
    double coefficient2;

    /**
     * determine the side of the tube
     */
    int coefficient3;

    vector<vtkSmartPointer<vtkPolyData>> dataList;

public:
    Graph();

    static Graph *New();

    /**
     * extend structural lines and generate intersections
     * @param tubes
     */
    void create(vector<vtkSmartPointer<Tube>> tubes);

    /**
     * blend tubes
     */
    void update();

    vector<vtkSmartPointer<vtkPolyData>> getOutput();

    void setLength(double length);

    vector<vtkSmartPointer<Intersection>> &getIntersections();

    void setCoefficient1(double coefficient1);

    void setCoefficient2(double coefficient2);

    void setCoefficient3(int coefficient3);

};


#endif //TUBE_CONNECT_GRAPH_H
