//
// Created by wzx on 18-11-22.
//

#include "../include/CellPickerInteractorStyle.h"

#include <vtkCellPicker.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkCellData.h>
#include <vtkCellArray.h>
#include <vtkPolyDataNormals.h>
#include <vtkPointPicker.h>
#include <vector>

using std::vector;

CellPickerInteractorStyle *CellPickerInteractorStyle::New() {
    return new CellPickerInteractorStyle;
}


CellPickerInteractorStyle::CellPickerInteractorStyle() {
    this->selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    this->selectedActor = vtkSmartPointer<vtkActor>::New();
    this->stlRender = vtkSmartPointer<STLRender>::New();
    this->cellID = 0;
}

void CellPickerInteractorStyle::OnLeftButtonDown() {
    if (this->selectMode) {

        int *pos = this->GetInteractor()->GetEventPosition();

        auto cellPicker = vtkSmartPointer<vtkCellPicker>::New();
        cellPicker->SetTolerance(0.0005);
        cellPicker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());
        this->cellID = cellPicker->GetCellId();

        if (this->cellID != -1) {
            auto ids = vtkSmartPointer<vtkIdTypeArray>::New();
            ids->SetNumberOfComponents(1);
            ids->InsertNextValue(this->cellID);

//            vtkIdType pointID;
//            pointID = this->stlRender->getData()->GetCell(this->cellID)->GetPointId(1);
//            double point[3];
//            this->stlRender->getData()->GetPoint(pointID, point);
//            for (int i = 0; i < 3; i++) {
//                cout.precision(5);
//                cout << point[i] << '\t';
//            }
//            cout << endl;

            auto selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
            selectionNode->SetFieldType(vtkSelectionNode::CELL);
            selectionNode->SetContentType(vtkSelectionNode::INDICES);
            selectionNode->SetSelectionList(ids);

            auto selection = vtkSmartPointer<vtkSelection>::New();
            selection->AddNode(selectionNode);

            auto extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
            extractSelection->SetInputData(0, this->stlRender->getData());
            extractSelection->SetInputData(1, selection);
            extractSelection->Update();

            selectedMapper->SetInputConnection(extractSelection->GetOutputPort());
            selectedActor->SetMapper(this->selectedMapper);
            selectedActor->GetProperty()->EdgeVisibilityOn();
            selectedActor->GetProperty()->SetEdgeColor(1, 0, 0);
            selectedActor->GetProperty()->SetLineWidth(3);

            this->stlRender->getRenderer()->AddActor(selectedActor);

        }
    }

    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void CellPickerInteractorStyle::setSelectMode(bool selectMode) {
    CellPickerInteractorStyle::selectMode = selectMode;
}

void CellPickerInteractorStyle::setStlRender(vtkSmartPointer<STLRender> stlRender) {
    CellPickerInteractorStyle::stlRender = stlRender;
    this->tubes = stlRender->getTubes();
}

