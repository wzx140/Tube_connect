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

public:
    Graph();

    static Graph *New();

    void create(vector<vtkSmartPointer<Tube>> tubes);

    vector<array<array<double, 3>, 2>> &getLines();

    void setRadius(double radius);

    void setLength(double length);

    vector<vtkSmartPointer<Intersection>> &getIntersections();
};


#endif //TUBE_CONNECT_GRAPH_H
