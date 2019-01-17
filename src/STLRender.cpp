//
// Created by wzx on 18-11-21.
//

#include "../include/STLRender.h"

#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkAxesActor.h>
#include <vtkProperty.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkAppendPolyData.h>

using std::string;
using std::vector;

STLRender *STLRender::New() {
    return new STLRender;
}


STLRender::STLRender() {
    this->interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    this->data = vtkSmartPointer<vtkPolyData>::New();
    this->renderer = vtkSmartPointer<vtkRenderer>::New();
    this->actor = vtkSmartPointer<vtkActor>::New();

    auto window = vtkSmartPointer<vtkRenderWindow>::New();
    window->AddRenderer(this->renderer);
    auto style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    this->interactor->SetRenderWindow(window);
    this->interactor->SetInteractorStyle(style);
}


void STLRender::setPath(const char *path) {
    STLRender::path = path;
}

void STLRender::load() {
    auto reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(this->path.c_str());
    reader->Update();
    this->data = reader->GetOutput();
}

vtkSmartPointer<vtkRenderer> STLRender::getRenderer() {
    return renderer;
}

void STLRender::start() {
    this->interactor->Initialize();
    this->interactor->Start();
}

void STLRender::axisOn() {
    auto axes = vtkSmartPointer<vtkAxesActor>::New();
    axes->SetShaftType(0);
//    the length of the three axes
    axes->SetTotalLength(2, 2, 2);
    axes->SetAxisLabels(1);

    this->renderer->AddActor(axes);
}

void STLRender::setStyle(vtkSmartPointer<vtkInteractorStyleTrackballCamera> style) {
    this->interactor->SetInteractorStyle(style);
}

vector<vtkSmartPointer<Tube>> STLRender::getTubes() {

    vector<vtkSmartPointer<Tube>> tubes;

    auto filter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
    filter->SetInputData(this->data);
    filter->SetExtractionModeToAllRegions();
    filter->Update();

    int num = filter->GetNumberOfExtractedRegions();

    for (int i = 0; i < num; i++) {
        auto tube = vtkSmartPointer<Tube>::New();

        auto filter2 = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
        filter2->SetInputData(this->data);
        filter2->InitializeSpecifiedRegionList();
        filter2->SetExtractionModeToSpecifiedRegions();
        filter2->AddSpecifiedRegion(i);
        filter2->Update();

        tube->setData(filter2->GetOutput());
        tubes.emplace_back(tube);
    }

    return tubes;
}

vtkSmartPointer<vtkPolyData> STLRender::getData() {
    return data;
}

void STLRender::setInputData(vtkSmartPointer<vtkPolyData> data, double opacity) {

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    this->data = data;
    mapper->SetInputData(data);
    mapper->Update();

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetOpacity(opacity);

    this->renderer->RemoveActor(this->actor);
    this->actor = actor;

    this->renderer->AddActor(actor);
}

void STLRender::setInputData(vector<vtkSmartPointer<vtkPolyData>> dataList, double opacity) {

    auto append = vtkSmartPointer<vtkAppendPolyData>::New();
    for (auto &data : dataList) {
        append->AddInputData(data);
    }
    append->Update();
    this->data = append->GetOutput();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(append->GetOutputPort());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetOpacity(opacity);

    this->renderer->RemoveActor(this->actor);
    this->actor = actor;
    this->renderer->AddActor(actor);
}

vtkSmartPointer<vtkPolyData> STLRender::append(const vector<vtkSmartPointer<vtkPolyData>> &dataList) {
    auto append = vtkSmartPointer<vtkAppendPolyData>::New();
    for (int i = 0; i < dataList.size(); i++) {
        append->AddInputData(dataList[i]);
    }
    append->Update();
    return append->GetOutput();
}
