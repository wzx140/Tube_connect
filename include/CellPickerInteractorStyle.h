//
// Created by wzx on 18-11-22.
//

#ifndef PIPESTITCHING_CELLPICKERINTERACTORSTYLE_H
#define PIPESTITCHING_CELLPICKERINTERACTORSTYLE_H


#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>

#include "STLRender.h"

/**
 * pick the cell
 */
class CellPickerInteractorStyle : public vtkInteractorStyleTrackballCamera {
private:

    vtkSmartPointer<STLRender> stlRender;

    vtkSmartPointer<vtkDataSetMapper> selectedMapper;

    vtkSmartPointer<vtkActor> selectedActor;

    /**
     * the condition of select mode
     */
    bool selectMode;

    /**
     * selected cell id
     */
    vtkIdType cellID;

    /**
     * all cylinders in data
     */
    std::vector<vtkSmartPointer<Cylinder>> cylinders;

    /**
     * calculate normals of data's cells, this must be called after function setData()
     * @param data
     * @return three components of tuple
     */
    vtkSmartPointer<vtkDataArray> calculateNormals(vtkSmartPointer<vtkPolyData> data);

    /**
     * only used by OnLeftButtonDown()
     * @param scale
     */
    int stretchSelectedCylinder(double point[3], double scale);

public:
    static CellPickerInteractorStyle *New();

    CellPickerInteractorStyle();

    void OnLeftButtonDown() override;

    /**
     * must be called after STLRender::load()
     * @param stlRender
     */
    void setStlRender(vtkSmartPointer<STLRender> stlRender);

    /**
     * to decide whether the left mouse click can select a cell
     * @param selectMode
     */
    void setSelectMode(bool selectMode);

};


#endif //PIPESTITCHING_CELLPICKERINTERACTORSTYLE_H
