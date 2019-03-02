//
// Created by wzx on 18-11-21.
//

#ifndef PIPESTITCHING_STRETCHENTITYFILTER_H
#define PIPESTITCHING_STRETCHENTITYFILTER_H


#include <vtkObject.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkDoubleArray.h>
#include <array>

using std::array;
using std::vector;


class Tube : public vtkObject {
private:

    /**
     * data of tube
     */
    vtkSmartPointer<vtkPolyData> data;

    /**
     * the positive-direction normal of the tube
     */
    array<double, 3> normal;

    /**
     * points in the cell
     */
    vector<array<double, 3>> points;

    /**
     * start center point of the tube
     */
    array<double, 3> stPoint;

    /**
     * end center point of the tube
     */
    array<double, 3> endPoint;

public:
    Tube();

    static Tube *New();

    /**
     * judge the point is on the tube
     * @param point
     * @return
     */
    bool hasPoint(array<double, 3> point);

    void setData(vtkSmartPointer<vtkPolyData> data);

    vtkSmartPointer<vtkPolyData> getData();

    array<double, 3> &getNormal();

    /**
     * this function must be called after set data
     * @return
     */
    const vector<array<double, 3>> &getPoints() const;

    array<array<double, 3>, 2> getStructureLine();

    void setNormal(array<double, 3> &normal);

    array<double, 3> &getStPoint();

    void setStPoint(array<double, 3> &stPoint);

    array<double, 3> &getEndPoint();

    void setEndPoint(array<double, 3> &endPoint);

};


#endif //PIPESTITCHING_STRETCHENTITYFILTER_H
