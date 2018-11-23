//
// Created by wzx on 18-11-21.
//

#include "../include/Cylinder.h"
#include <vtkTransform.h>

using std::cout;

Cylinder *Cylinder::New() {
    return new Cylinder;
}

Cylinder::Cylinder() {
    this->data = vtkSmartPointer<vtkPolyData>::New();
}

void Cylinder::stretch(double scale, const double vector[], const double basePoint[]) {

    auto transform = vtkSmartPointer<vtkTransform>::New();

    transform->PostMultiply();

    double vectorNorm[3];
    double norm = vtkMath::Norm(vector);
    for (int i = 0; i < 3; i++) {
        vectorNorm[i] = vector[i] / norm;
    }

//    transfer to orgin
    transform->Translate(-basePoint[0], -basePoint[1], -basePoint[2]);


//    rotate to (0,0,1) and back
    double z[3] = {0.0, 0.0, 1.0};
    auto theta = vtkMath::DegreesFromRadians(acos(vtkMath::Dot(vectorNorm, z)));
//    cout << theta;
    double axis[3];
    vtkMath::Cross(vector, z, axis);
    transform->RotateWXYZ(theta, axis);

    transform->Scale(1, 1, scale);

//    undo rotate
    axis[3];
    vtkMath::Cross(z, vector, axis);
    transform->RotateWXYZ(theta, axis);

//    transfer to basePoint
    transform->Translate(basePoint[0], basePoint[1], basePoint[2]);

    auto filter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();

    filter->SetInputData(this->data);
    filter->SetTransform(transform);
    filter->Update();
    this->data = filter->GetOutput();
}

void Cylinder::setData(vtkSmartPointer<vtkPolyData> data) {
    Cylinder::data = data;
}

vtkSmartPointer<vtkPolyData> Cylinder::getData() {
    return data;
}

bool Cylinder::hasPoint(double point[]) {
    int num = this->data->GetNumberOfCells();
    for (int i = 0; i < num; i++) {

        for (int j = 0; j < this->data->GetCell(i)->GetNumberOfPoints(); j++) {

            double point_[3];
            this->data->GetPoint(this->data->GetCell(i)->GetPointId(j), point_);

//            for (int k = 0; k < 3; k++) {
//                cout<<point_[i]<<'\t';
//            }
//            cout << '\n';

            double epsilon = 0.0001;

            if (abs(point_[0] - point[0]) < epsilon &&
                abs(point_[1] - point[1]) < epsilon &&
                abs(point_[2] - point[2]) < epsilon) {
                return true;
            }
        }
    }
    return false;
}

