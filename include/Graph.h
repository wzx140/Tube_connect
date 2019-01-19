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
     * lines in graph
     */
    vector<array<array<double, 3>, 2>> lines;

    /**
     * intersections in graph
     */
    vector<vtkSmartPointer<Intersection>> intersections;

    /**
     * radius of the tube
     */
    double radius;

    /**
     * the length to extend
     */
    double length;

    /**
     * determine the room in intersections
     */
    double coefficient1;

    /**
     * determine the room in intersections
     */
    double coefficient2;

    /**
     * determine the resolution like weight
     */
    double coefficient3;

    /**
     * determine the resolution like bias
     */
    double coefficient4;

private:

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

    /**
     * 0->tubes , others->connections
     * @param i
     * @return
     */
    vtkSmartPointer<vtkPolyData> getOutput(int i);

    vector<array<array<double, 3>, 2>> &getLines();

    void setRadius(double radius);

    void setLength(double length);

    vector<vtkSmartPointer<Intersection>> &getIntersections();

    void setCoefficient1(double coefficient1);

    void setCoefficient2(double coefficient2);

    void setCoefficient3(double coefficient3);

    void setCoefficient4(double coefficient4);

};


#endif //TUBE_CONNECT_GRAPH_H
