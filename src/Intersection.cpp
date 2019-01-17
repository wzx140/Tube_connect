//
// Created by wzx on 19-1-11.
//

#include "../include/Intersection.h"

Intersection::Intersection() = default;

bool Intersection::hasId(int id) {
    for (int i = 0; i < this->info.size(); i++) {
        int item = this->info.at(i).first;
        if (item == id) {
            return true;
        }
    }
    return false;
}

Intersection *Intersection::New() {
    return new Intersection;
}

array<double, 3> &Intersection::getPoint() {
    return point;
}

void Intersection::setPoint(double x, double y, double z) {
    Intersection::point[0] = x;
    Intersection::point[1] = y;
    Intersection::point[2] = z;
}

vector<vtkSmartPointer<Tube>> &Intersection::getTubes() {
    return tubes;
}

void Intersection::setTubes(vector<vtkSmartPointer<Tube>> &tubes) {
    Intersection::tubes = tubes;
}

bool Intersection::hasPoint(array<double, 3> &point) {
    double epsilon = 0.1;
    if (abs(point[0] - this->point[0]) < epsilon &&
        abs(point[1] - this->point[1]) < epsilon &&
        abs(point[2] - this->point[2]) < epsilon) {
        return true;
    }
    return false;
}

void Intersection::addTube(vtkSmartPointer<Tube> tube) {
    this->tubes.emplace_back(tube);
}

void Intersection::addId(int id) {
    pair<int, double> a(id, 0);

    this->info.push_back(a);
}

vector<pair<int, double>> &Intersection::getInfo() {
    return info;
}

double Intersection::getRoom(int id) {
    for (int i = 0; i < this->info.size(); i++) {
        if (this->info.at(i).first == id) {
            return this->info.at(i).second;
        }
    }
    return 0;
}


