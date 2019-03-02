//
// Created by wzx on 18-11-21.
//

#include "../include/Tube.h"
#include <vtkTransform.h>

#include "../util/CircleUtil.h"
#include "../util/TubeUtil.h"

using std::cout;

Tube *Tube::New() {
    return new Tube;
}

Tube::Tube() {
    this->data = vtkSmartPointer<vtkPolyData>::New();
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

array<double, 3> &Tube::getNormal() {
    return normal;
}

const vector<array<double, 3>> &Tube::getPoints() const {
    return points;
}

array<array<double, 3>, 2> Tube::getStructureLine() {
    //    calculate tubeNormal
    array<array<double, 3>, 2> line{};
    auto normals = VectorUtil::calculateNormals(this->data);
    array<double, 3> normal1 = {0};
    normals->GetTuple(0, normal1.data());
    auto normal2 = VectorUtil::getDifferentVector(normals, normal1);

    array<double, 3> tubeNormal = {0};
    vtkMath::Cross(normal1.data(), normal2.data(), tubeNormal.data());

    auto edgePoints = TubeUtil::getEdgePoint(this->points, tubeNormal);
    auto center1 = CircleUtil::getCenter(edgePoints[0], edgePoints[1], edgePoints[2], tubeNormal);

    VectorUtil::reverse(tubeNormal);
    edgePoints = TubeUtil::getEdgePoint(this->points, tubeNormal);
    auto center2 = CircleUtil::getCenter(edgePoints[0], edgePoints[1], edgePoints[2], tubeNormal);

    line[0] = center1;
    line[1] = center2;

    return line;
}

void Tube::setNormal(array<double, 3> &normal) {
    VectorUtil::regularize(normal);
    this->normal = normal;
}

array<double, 3> &Tube::getStPoint() {
    return stPoint;
}

void Tube::setStPoint(array<double, 3> &stPoint) {
    Tube::stPoint = stPoint;
}

array<double, 3> &Tube::getEndPoint() {
    return endPoint;
}

void Tube::setEndPoint(array<double, 3> &endPoint) {
    Tube::endPoint = endPoint;
}

