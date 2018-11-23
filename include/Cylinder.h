//
// Created by wzx on 18-11-21.
//

#ifndef PIPESTITCHING_STRETCHENTITYFILTER_H
#define PIPESTITCHING_STRETCHENTITYFILTER_H


#include <vtkObject.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkTransformPolyDataFilter.h>


class Cylinder : public vtkObject {
private:

    /**
     * data of cylinder
     */
    vtkSmartPointer<vtkPolyData> data;


public:
    Cylinder();

    static Cylinder *New();

    /**
     * stretch the cylinder, rotate the cylinder to set that Z-axis is the stretch direction and rotate back to default
     * @param scale: the scala to stretch the cylinder
     * @param vector: the stretch direction
     * @param basePoint: defined the center of scala origin
     */
    void stretch(double scale, const double vector[], const double basePoint[]);

    /**
     * judge the point is on the cylinder
     * @param point
     * @return
     */
    bool hasPoint(double point[]);

    void setData(vtkSmartPointer<vtkPolyData> data);

    vtkSmartPointer<vtkPolyData> getData();
};


#endif //PIPESTITCHING_STRETCHENTITYFILTER_H
