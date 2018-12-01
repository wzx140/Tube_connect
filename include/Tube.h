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


class Tube : public vtkObject {
private:

    /**
     * data of tube
     */
    vtkSmartPointer<vtkPolyData> data;

    /**
     * the positive-direction normal of the tube
     */
    double *normal;

    /**
     * the edge of the tube used to connect curved face
     */
    vtkSmartPointer<vtkDoubleArray> edgePoints;

public:
    Tube();

    static Tube *New();

    /**
     * judge the point is on the cylinder
     * @param point
     * @return
     */
    bool hasPoint(double point[]);

    /**
     * this fuction must be called after set date
     * @param point: used to calculate the positive-direction normal of the tube, it must on one of the other tube
     */
    void update(double point[]);

    void setData(vtkSmartPointer<vtkPolyData> data);

    vtkSmartPointer<vtkPolyData> getData();

    double *getNormal();

    vtkSmartPointer<vtkDoubleArray> getEdgePoints();

    virtual ~Tube();
};


#endif //PIPESTITCHING_STRETCHENTITYFILTER_H
