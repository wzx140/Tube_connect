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
     * points in the cell
     */
    vector<array<double, 3>> points;

    /**
     * the center line of the tube
     */
    array<array<double, 3>, 2> centerLine;

    /**
     * the radius of the tube
     */
    double radius;

    /**
     * the normal of the tube
     */
    array<double, 3> normal;

    /**
     * the bounds of the tube
     * min and max of xyz
     */
    array<double, 6> bounds;

    /**
     * 0 -> mode with poly data
     * 1 -> mode with center line
     * this affect the update function
     */
    int mode;

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

    /**
     * this function must be called after set data
     * @return
     */
    const vector<array<double, 3>> &getPoints() const;

    array<array<double, 3>, 2> getStructureLine();

    double getRadius();

    void setRadius(double radius);

    array<array<double, 3>, 2> &getCenterLine();

    void setCenterLine(array<array<double, 3>, 2> &centerLine);

    int getMode();

    void setMode(int mode);

    array<double, 6> &getBounds();

    /**
     * see mode
     */
    void update();

};


#endif //PIPESTITCHING_STRETCHENTITYFILTER_H
