//
// Created by wzx on 18-11-21.
//

#include "../include/Tube.h"
#include <vtkTransform.h>
#include <vtkCleanPolyData.h>

#include "../util/CircleUtil.h"
#include "../util/TubeUtil.h"

using std::cout;

Tube *Tube::New() {
    return new Tube;
}

Tube::Tube() {
    this->bounds = {0, 0, 0, 0, 0, 0};
}


void Tube::setData(vtkSmartPointer<vtkPolyData> data) {
    Tube::data = data;
}

vtkSmartPointer<vtkPolyData> Tube::getData() {
    return data;
}

bool Tube::hasPoint(array<double, 3> point) {
    for (int i = 0; i < this->points.size(); i++) {

        auto pointIn = this->points[i];
        double epsilon = 0.001;

        if (abs(pointIn[0] - point[0]) < epsilon &&
            abs(pointIn[1] - point[1]) < epsilon &&
            abs(pointIn[2] - point[2]) < epsilon) {
            return true;
        }
    }
    return false;
}

const vector<array<double, 3>> &Tube::getPoints() const {
    return points;
}

array<array<double, 3>, 2> Tube::getStructureLine() {
    auto edgePoints = TubeUtil::getEdgePoint(this->points, this->normal);
    auto center1 = CircleUtil::getCenter(edgePoints[0], edgePoints[1], edgePoints[2], this->normal);

    VectorUtil::reverse(this->normal);
    edgePoints = TubeUtil::getEdgePoint(this->points, this->normal);
    auto center2 = CircleUtil::getCenter(edgePoints[0], edgePoints[1], edgePoints[2], this->normal);

    array<array<double, 3>, 2> line{};
    line[0] = center1;
    line[1] = center2;

    return line;
}

double Tube::getRadius() {
    return radius;
}

void Tube::setRadius(double radius) {
    Tube::radius = radius;
}

array<array<double, 3>, 2> &Tube::getCenterLine() {
    return centerLine;
}

void Tube::setCenterLine(array<array<double, 3>, 2> &centerLine) {
    Tube::centerLine = centerLine;
}

void Tube::update() {
    if (mode == 0) {

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

//    get radius
        auto normals = VectorUtil::calculateNormals(this->data);
        array<double, 3> normal1 = {0};
        normals->GetTuple(0, normal1.data());
        auto normal2 = VectorUtil::getDifferentVector(normals, normal1);
        array<double, 3> tubeNormal = {0};
        vtkMath::Cross(normal1.data(), normal2.data(), tubeNormal.data());
        this->normal = tubeNormal;
        auto edgePoints = TubeUtil::getEdgePoint(this->points, tubeNormal);
        auto center = CircleUtil::getCenter(edgePoints[0], edgePoints[1], edgePoints[2], tubeNormal);
        this->radius = LineUtil::getLength(center, edgePoints[0]);

    } else if (mode == 1) {
        auto line = vtkSmartPointer<vtkLineSource>::New();
        line->SetPoint1(this->centerLine[0].data());
        line->SetPoint2(this->centerLine[1].data());
        auto tube = vtkSmartPointer<vtkTubeFilter>::New();
        tube->SetNumberOfSides(50);
        tube->SetRadius(this->radius);
        tube->SetCapping(true);
        tube->SetInputConnection(line->GetOutputPort());
        tube->Update();
        tube->GetOutput()->GetBounds(this->bounds.data());
    }

}

int Tube::getMode() {
    return mode;
}

void Tube::setMode(int mode) {
    assert(mode == 0 || mode == 1);
    Tube::mode = mode;
}

array<double, 6> &Tube::getBounds() {
    return bounds;
}

