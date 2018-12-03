//
// Created by wzx on 18-11-21.
//

#include "../include/Tube.h"
#include <vtkTransform.h>
#include <vtkMath.h>
#include <vtkPoints.h>
#include <vtkIdList.h>

#include "../util/VectorUtil.h"

using std::cout;

Tube *Tube::New() {
    return new Tube;
}

Tube::Tube() {
    this->data = vtkSmartPointer<vtkPolyData>::New();
    this->edgePoints = vtkSmartPointer<vtkDoubleArray>::New();
    this->edgePoints->SetNumberOfComponents(3);
    this->points = vtkSmartPointer<vtkDoubleArray>::New();
    this->points->SetNumberOfComponents(3);
    this->normal = {0};
}


void Tube::setData(vtkSmartPointer<vtkPolyData> data) {
    Tube::data = data;
    auto polys = data->GetPolys();
    vtkIdType npts = 0, *pts;
    for (int i = 0; i < data->GetNumberOfPolys(); i++) {
        int h = polys->GetNextCell(npts, pts);
        if (h == 0) {
            break;
        } else if (npts == 3) {
            this->points->InsertNextTuple(data->GetPoint(pts[0]));
            this->points->InsertNextTuple(data->GetPoint(pts[1]));
            this->points->InsertNextTuple(data->GetPoint(pts[2]));
        }
    }
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
    this->points->GetTuple(0, pointIn.data());
    auto normalCompare = VectorUtil::getVector(pointIn, point);
    double degree = VectorUtil::getAngle(tubeNormal, normalCompare);
    if (degree > 90) {
        VectorUtil::reverseVector(tubeNormal);
    }
    VectorUtil::regularize(tubeNormal);

    this->normal = tubeNormal;


//    extract edge of the tube

    int pointID = 0;
    array<double, 3> edgePoint = {0};
    array<double, 3> pointTemp = {0};
    for (int i = 0; i < this->points->GetNumberOfTuples(); i++) {

        bool flag = true;
        this->points->GetTuple(i, edgePoint.data());

        for (int j = 0; j < this->points->GetNumberOfTuples(); j++) {
            if (i != j) {
                this->points->GetTuple(j, pointTemp.data());
                auto vector = VectorUtil::getVector(edgePoint, pointTemp);
                double angle = VectorUtil::getAngle(vector, this->normal);
                if (angle < 90 && angle > 0) {
                    flag = false;
                    break;
                }
            }
        }

        if (flag) {
            this->edgePoints->InsertNextTuple(edgePoint.data());
            pointID = i;
            break;
        }
    }

    for (int i = 0; i < this->points->GetNumberOfTuples(); i++) {
        if (i != pointID) {
            this->points->GetTuple(i, pointTemp.data());
            auto vector = VectorUtil::getVector(pointTemp, edgePoint);
            if (abs(VectorUtil::getAngle(vector, this->normal) - 90) < 0.01) {
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

const vtkSmartPointer<vtkDoubleArray> Tube::getPoints() const {
    return points;
}

void Tube::setResolution(int resolution) {
    Tube::resolution = resolution;
}


