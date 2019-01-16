//
// Created by wzx on 19-1-11.
//

#ifndef TUBE_CONNECT_INTERSECTION_H
#define TUBE_CONNECT_INTERSECTION_H

#include <vtkObject.h>
#include <array>
#include <vector>

#include "../include/Tube.h"

using std::vector;
using std::array;

/**
 * the intersection and its info
 */
class Intersection : public vtkObject {
private:
    array<double, 3> point;
    vector<vtkSmartPointer<Tube>> tubes;

    /**
     * index of tubes
     */
    vector<int> ids;

public:
    Intersection();

    static Intersection *New();

    array<double, 3> &getPoint();

    void setPoint(double x, double y, double z);

    vector<vtkSmartPointer<Tube>> &getTubes();

    void setTubes(vector<vtkSmartPointer<Tube>> &tubes);

    void addTube(vtkSmartPointer<Tube> tube);

    bool hasPoint(array<double, 3> &point);

    void addId(int id);

    bool hasId(int id);

};


#endif //TUBE_CONNECT_INTERSECTION_H
