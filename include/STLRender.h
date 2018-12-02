//
// Created by wzx on 18-11-21.
//

#ifndef PIPESTITCHING_STLRENDER_H
#define PIPESTITCHING_STLRENDER_H

#include <string>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkAlgorithmOutput.h>
#include <vector>

#include "Tube.h"

/**
 * used to render the stl file
 */
class STLRender : public vtkObject {
private:
    /**
     * the path of the stl file
     */
    std::string path;

    /**
     * the interactor window class ImageInteractionCallback may use to refresh
     */
    vtkSmartPointer<vtkRenderWindowInteractor> interactor;

    /**
     * data of stl file
     */
    vtkSmartPointer<vtkPolyData> data;

    vtkSmartPointer<vtkRenderer> renderer;

    vtkSmartPointer<vtkActor> actor;

public:
    static STLRender *New();

    STLRender();

    void setPath(const char *path);

    /**
     * load the data from stl file
     */
    void load();

    /**
     * start to display in window
     */
    void start();

    /**
     * display axis on the screen
     */
    void axisOn();

    void setInputData(vtkSmartPointer<vtkPolyData> data, double opacity);

    void setInputData(std::vector<vtkSmartPointer<vtkPolyData>> dataList, double opacity);

    /**
     * this function must be called before function load
     * @param style
     */
    void setStyle(vtkSmartPointer<vtkInteractorStyleTrackballCamera> style);

    /**
     * split the tubes in the data.
     * the data must have and only have tubes.
     * this function must be called after load()
     * @return
     */
    std::vector<vtkSmartPointer<Tube>> getTubes();

    /**
     * set actor transparent, default is true if you do not call this function
     * @param isTrue
     */
    void setTransparent(bool isTrue);

    vtkSmartPointer<vtkRenderer> getRenderer();

    vtkSmartPointer<vtkPolyData> getData();

};


#endif //PIPESTITCHING_STLRENDER_H
