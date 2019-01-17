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
     * the edge of the tube used to connect curved face
     */
    vector<array<double, 3>> edgePoints;

    /**
     * points in the cell
     */
    vector<array<double, 3>> points;

    /**
     * the number of the point in edge points
     */
    int resolution;

public:
    Tube();

    static Tube *New();

    /**
     * judge the point is on the tube
     * @param point
     * @return
     */
    bool hasPoint(array<double, 3> point);

    /**
     * calculate the edge point
     * @param normal: normal of the tube
     * @param center: center of the edge point
     * @param radius: radius of the edge
     */
    void update(array<double, 3> &normal, array<double, 3> &center, double radius);

    void setData(vtkSmartPointer<vtkPolyData> data);

    vtkSmartPointer<vtkPolyData> getData();

    array<double, 3> &getNormal();

    vector<array<double, 3>> &getEdgePoints();

    /**
     * this function must be called after set data
     * @return
     */
    const vector<array<double, 3>> &getPoints() const;

    void setResolution(int resolution);

    array<array<double, 3>, 2> getStructureLine();

};


#endif //PIPESTITCHING_STRETCHENTITYFILTER_H
