//
// Created by wzx on 18-11-21.
//

#include "../include/Tube.h"
#include <vtkTransform.h>
#include <vtkMath.h>
#include <vtkPoints.h>

#include "../util/VectorUtil.h"

using std::cout;

Tube *Tube::New() {
    return new Tube;
}

Tube::Tube() {
    this->data = vtkSmartPointer<vtkPolyData>::New();
    this->edgePoints = vtkSmartPointer<vtkDoubleArray>::New();
    this->edgePoints->SetNumberOfComponents(3);
    this->normal = {0};
}


void Tube::setData(vtkSmartPointer<vtkPolyData> data) {
    Tube::data = data;
}

vtkSmartPointer<vtkPolyData> Tube::getData() {
    return data;
}

bool Tube::hasPoint(array<double, 3> point) {
    for (int i = 0; i < this->data->GetNumberOfCells(); i++) {

        for (int j = 0; j < this->data->GetCell(i)->GetNumberOfPoints(); j++) {

            array<double, 3> pointIn = {0};
            this->data->GetPoint(this->data->GetCell(i)->GetPointId(j), pointIn.data());

//            for (int k = 0; k < 3; k++) {
//                cout<<pointIn[i]<<'\t';
//            }
//            cout << '\n';

            double epsilon = 0.001;

            if (abs(pointIn[0] - point[0]) < epsilon &&
                abs(pointIn[1] - point[1]) < epsilon &&
                abs(pointIn[2] - point[2]) < epsilon) {
                return true;
            }
        }
    }
    return false;
}

void Tube::update(array<double, 3> &point) {
//    calculate tubeNormal
    auto normals = VectorUtil::calculateNormals(this->data);
    array<double, 3> normal1 = {0};
    normals->GetTuple(0, normal1.data());
    auto normal2 = VectorUtil::getDifferentVector(normals, normal1);

    array<double, 3> tubeNormal = {0};
    vtkMath::Cross(normal1.data(), normal2.data(), tubeNormal.data());

    array<double, 3> pointIn = {0};
    this->data->GetPoint(0, pointIn.data());
    auto normalCompare = VectorUtil::getVector(pointIn, point);
    double degree = VectorUtil::getAngle(tubeNormal, normalCompare);
    if (degree > 90) {
        VectorUtil::reverseVector(tubeNormal);
    }
    VectorUtil::regularize(tubeNormal);

    this->normal = tubeNormal;


//    extract edge of the tube

    auto points = this->data->GetPoints();
    int pointID = 0;
    array<double, 3> edgePoint = {0};
    array<double, 3> pointTemp = {0};
    for (int i = 0; i < points->GetNumberOfPoints(); i++) {

        int flag = 0;
        points->GetPoint(i, edgePoint.data());

        for (int j = 0; j < points->GetNumberOfPoints(); j++) {
            if (i != j) {
                points->GetPoint(j, pointTemp.data());
                auto vector = VectorUtil::getVector(edgePoint, pointTemp);
                if (VectorUtil::getAngle(vector, this->normal) < 90) {
                    break;
                } else {
                    flag++;
                }
            }
        }

        if (flag == points->GetNumberOfPoints() - 1) {
            this->edgePoints->InsertNextTuple(edgePoint.data());
            pointID = i;
            break;
        }
    }

    for (int i = 0; i < points->GetNumberOfPoints(); i++) {
        if (i != pointID) {
            points->GetPoint(i, pointTemp.data());
            auto vector = VectorUtil::getVector(pointTemp, edgePoint);
            if (abs(VectorUtil::getAngle(vector, this->normal) - 90) < 0.001) {
                this->edgePoints->InsertNextTuple(pointTemp.data());
            }
        }
    }

}


vtkSmartPointer<vtkDoubleArray> Tube::getEdgePoints() {
    return edgePoints;
}

const array<double, 3> &Tube::getNormal() const {
    return normal;
}

