//
// Created by wzx on 18-11-21.
//

#include "../include/Tube.h"
#include <vtkTransform.h>
#include <vtkMath.h>
#include <vtkPoints.h>

#include "../util/CellUtil.h"
#include "../util/VectorUtil.h"

using std::cout;

Tube *Tube::New() {
    return new Tube;
}

Tube::Tube() {
    this->data = vtkSmartPointer<vtkPolyData>::New();
    this->edgePoints = vtkSmartPointer<vtkDoubleArray>::New();
    this->edgePoints->SetNumberOfComponents(3);
    this->normal = new double[3];
}


void Tube::setData(vtkSmartPointer<vtkPolyData> data) {
    Tube::data = data;
}

vtkSmartPointer<vtkPolyData> Tube::getData() {
    return data;
}

bool Tube::hasPoint(double point[]) {
    int num = this->data->GetNumberOfCells();
    for (int i = 0; i < num; i++) {

        for (int j = 0; j < this->data->GetCell(i)->GetNumberOfPoints(); j++) {

            double point_[3];
            this->data->GetPoint(this->data->GetCell(i)->GetPointId(j), point_);

//            for (int k = 0; k < 3; k++) {
//                cout<<point_[i]<<'\t';
//            }
//            cout << '\n';

            double epsilon = 0.001;

            if (abs(point_[0] - point[0]) < epsilon &&
                abs(point_[1] - point[1]) < epsilon &&
                abs(point_[2] - point[2]) < epsilon) {
                return true;
            }
        }
    }
    return false;
}

void Tube::update(double point[]) {
//    calculate tubeNormal
    auto normals = CellUtil::calculateNormals(this->data);
    double normal1[3];
    normals->GetTuple(0, normal1);
    double *normal2 = VectorUtil::getDifferentVector(normals, normal1);

    double tubeNormal[3];
    vtkMath::Cross(normal1, normal2, tubeNormal);

    double pointIn[3];
    this->data->GetPoint(0, pointIn);
    double normalCompare[3];
    VectorUtil::getVector(pointIn, point, normalCompare);
    double degree = VectorUtil::getAngle(tubeNormal, normalCompare);
    if (degree > 90) {
        VectorUtil::reverseVector(tubeNormal);
    }
    VectorUtil::regularize(tubeNormal);

    for (int i = 0; i < 3; i++) {
        this->normal[i] = tubeNormal[i];
    }


//    extract edge of the tube

    auto points = this->data->GetPoints();
    int pointID = 0;
    {
        double point[3];
        double vector[3];
        for (int i = 0; i < points->GetNumberOfPoints(); i++) {

            int flag = 0;
            points->GetPoint(i, point);

            for (int j = 0; j < points->GetNumberOfPoints(); j++) {
                if (i != j) {
                    double point2[3];
                    points->GetPoint(j, point2);
                    VectorUtil::getVector(point2, point, vector);
                    if (VectorUtil::getAngle(vector, this->normal) > 90) {
                        break;
                    } else {
                        flag++;
                    }
                }
            }

            if (flag == points->GetNumberOfPoints() - 1) {
                this->edgePoints->InsertNextTuple3(point[0], point[1], point[2]);
                pointID = i;
                break;
            }
        }
    }

    double vector[3];
    for (int i = 0; i < points->GetNumberOfPoints(); i++) {
        if (i != pointID) {
            auto point = points->GetPoint(i);
            VectorUtil::getVector(point, this->edgePoints->GetTuple3(0), vector);
            if (abs(VectorUtil::getAngle(vector, this->normal) - 90) < 0.001) {
                this->edgePoints->InsertNextTuple3(point[0], point[1], point[2]);
            }
        }
    }

}

double *Tube::getNormal() {
    return normal;
}

vtkSmartPointer<vtkDoubleArray> Tube::getEdgePoints() {
    return edgePoints;
}

Tube::~Tube() {
    delete this->normal;
}

