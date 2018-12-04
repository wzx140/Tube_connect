//
// Created by wzx on 18-11-21.
//

#include "../include/Tube.h"
#include <vtkTransform.h>

#include "../util/CircleUtil.h"

using std::cout;

Tube *Tube::New() {
    return new Tube;
}

Tube::Tube() {
    this->data = vtkSmartPointer<vtkPolyData>::New();
    this->edgePoints = vtkSmartPointer<vtkDoubleArray>::New();
    this->edgePoints->SetNumberOfComponents(3);
    this->resolution = 50;
    this->normal = {0};
}


void Tube::setData(vtkSmartPointer<vtkPolyData> data) {
    Tube::data = data;

//    get points in cells
    auto polys = data->GetPolys();
    array<double, 3> point{};
    vtkIdType npts = 0, *pts;
    for (int i = 0; i < data->GetNumberOfPolys(); i++) {
        int h = polys->GetNextCell(npts, pts);
        if (h == 0) {
            break;
        } else if (npts == 3) {
            data->GetPoint(pts[0], point.data());
            if (!this->hasPoint(point)) {
                this->points.push_back(point);
            }
            data->GetPoint(pts[1], point.data());
            if (!this->hasPoint(point)) {
                this->points.push_back(point);
            }
            data->GetPoint(pts[2], point.data());
            if (!this->hasPoint(point)) {
                this->points.push_back(point);
            }

        }
    }
//    for (int i = 0; i < points.size(); i++) {
//        auto point = points[i];
//        cout << point[0] << '\t' << point[1] << '\t' << point[2] << endl;
//    }


}

vtkSmartPointer<vtkPolyData> Tube::getData() {
    return data;
}

bool Tube::hasPoint(array<double, 3> point) {
    for (int i = 0; i < this->points.size(); i++) {

        auto pointIn = this->points[i];

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

    auto pointIn = this->points[0];
    auto normalCompare = VectorUtil::getVector(pointIn, point);
    double degree = VectorUtil::getAngle(tubeNormal, normalCompare);
    if (degree > 90) {
        VectorUtil::reverseVector(tubeNormal);
    }
    VectorUtil::regularize(tubeNormal);

    this->normal = tubeNormal;


//    extract edge of the tube

    int pointID = 0;
    array<double, 3> edgePoint1 = {0};
    array<double, 3> edgePoint2 = {0};
    array<double, 3> edgePoint3 = {0};

    for (int i = 0; i < this->points.size(); i++) {

        bool flag = true;
        edgePoint1 = this->points[i];

        for (int j = 0; j < this->points.size(); j++) {
            if (i != j) {
                auto pointTemp = this->points[j];
                auto vector = VectorUtil::getVector(edgePoint1, pointTemp);
                double angle = VectorUtil::getAngle(vector, this->normal);
                if (angle < 90 && angle > 0) {
                    flag = false;
                    break;
                }
            }
        }
        if (flag) {
            pointID = i;
            break;
        }
    }

//  get three point in the edge
    int pointNum = 2;
    for (int i = 0; i < this->points.size(); i++) {
        if (i != pointID) {
            auto point = this->points[i];
            auto vector = VectorUtil::getVector(point, edgePoint1);
            if (abs(VectorUtil::getAngle(vector, this->normal) - 90) < 0.01) {
                if (pointNum == 2) {
                    edgePoint2 = point;
                    pointNum--;
                } else if (pointNum == 1) {
                    edgePoint3 = point;
                    pointNum--;
                } else if (pointNum == 0) {
                    break;
                }
            }
        }
    }
    this->edgePoints = CircleUtil::getCircle(edgePoint1, edgePoint2, edgePoint3, this->normal, this->resolution);

}

vtkSmartPointer<vtkDoubleArray> Tube::getEdgePoints() {
    return edgePoints;
}

const array<double, 3> &Tube::getNormal() const {
    return normal;
}

const vector<array<double, 3>> &Tube::getPoints() const {
    return points;
}

void Tube::setResolution(int resolution) {
    Tube::resolution = resolution;
}
