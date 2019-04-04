//
// Created by wzx on 19-1-11.
//

#include <cmath>

#include "../util/TubeUtil.h"
#include "../include/Graph.h"
#include "../include/STLRender.h"
#include <vtkContourFilter.h>
#include <vtkSampleFunction.h>

using std::pair;
using std::sort;

/**
 * catch error
 */
class Observer : public vtkCommand {
public:
    bool hasError;
    std::string msg;

    Observer() : hasError(false) {}

    static Observer *New() {
        return new Observer;
    }

    virtual void Execute(vtkObject *vtkNotUsed(caller), unsigned long event, void *calldata) {
        hasError = event == vtkCommand::ErrorEvent;
        msg = static_cast<char *>(calldata);
    }

    void Clear() {
        hasError = false;
        msg.clear();
    }
};


Graph::Graph() {
    this->length = 100;
    this->radius = 3;
    this->coefficient1 = 2;
    this->coefficient2 = 2;
    this->coefficient3 = 30;
}

Graph *Graph::New() {
    return new Graph;
}

void Graph::create(vector<vtkSmartPointer<Tube>> tubes) {
    vector<array<array<double, 3>, 2>> lines{};

    for (const auto &tube : tubes) {
        lines.emplace_back(tube->getStructureLine());
    }

    // combine the same direction line
    for (int i = 0; i < lines.size(); i++) {
        for (int j = i + 1; j < lines.size(); j++) {
            if (LineUtil::isCollinear(lines.at(i), lines.at(j))) {
                double length1 = LineUtil::getLength(lines.at(i)[0], lines.at(j)[0]);
                double length2 = LineUtil::getLength(lines.at(i)[0], lines.at(j)[1]);
                double length3 = LineUtil::getLength(lines.at(i)[1], lines.at(j)[0]);
                double length4 = LineUtil::getLength(lines.at(i)[1], lines.at(j)[1]);

                // find minimum
                double min = length1;
                array<double, 3> stPoint = lines.at(i)[1];
                array<double, 3> endPoint = lines.at(j)[1];
                if (length2 < min) {
                    min = length2;
                    stPoint = lines.at(i)[1];
                    endPoint = lines.at(j)[0];
                }
                if (length3 < min) {
                    min = length3;
                    stPoint = lines.at(i)[0];
                    endPoint = lines.at(j)[1];
                }
                if (length4 < min) {
                    min = length4;
                    stPoint = lines.at(i)[0];
                    endPoint = lines.at(j)[0];
                }

                if (min <= this->length) {
                    lines.erase(lines.begin() + i);
                    lines.erase(lines.begin() + j - 1);

                    array<array<double, 3>, 2> line{stPoint, endPoint};
                    lines.emplace_back(line);
                    i--;
                    break;
                }
            }
        }
    }

    // extend the lines
    for (int i = 0; i < lines.size(); i++) {
        LineUtil::extend(lines.at(i)[0], lines.at(i)[1], this->length);
//        this->lines.emplace_back(lines.at(i));
    }

    // get cross points
    array<double, 3> point{};
    for (int i = 0; i < lines.size(); i++) {
        for (int j = i + 1; j < lines.size(); j++) {

            if (LineUtil::intersection3D(lines[i][0], lines[i][1], lines[j][0], lines[j][1], point)) {

                int flag = 0;
                for (int k = 0; k < this->intersections.size(); k++) {
                    if (this->intersections[k]->hasPoint(point)) {
                        flag = 1;

                        if (this->intersections[k]->hasId(i) && !this->intersections[k]->hasId(j)) {
                            this->intersections[k]->addId(j);

                        } else if (this->intersections[k]->hasId(j) && !this->intersections[k]->hasId(i)) {
                            this->intersections[k]->addId(i);
                        }
                        break;
                    }
                }
                if (flag == 0) {
                    auto intersection = vtkSmartPointer<Intersection>::New();
                    intersection->addId(i);
                    intersection->addId(j);
                    intersection->setPoint(point[0], point[1], point[2]);

                    this->intersections.emplace_back(intersection);
                }
            }
        }
    }

    // Leave dynamic room
    for (int i = 0; i < this->intersections.size(); i++) {
        auto &info = this->intersections.at(i)->getInfo();
        auto point = intersections.at(i)->getPoint();

        for (int j = 0; j < info.size(); j++) {

            double minAngle = 90.;
            auto vector1 = VectorUtil::getVector(lines.at(info.at(j).first)[0], lines.at(info.at(j).first)[1]);
            for (int k = 0; k < info.size(); k++) {
                if (k == j) { continue; }
                auto vector2 = VectorUtil::getVector(lines.at(info.at(k).first)[0], lines.at(info.at(k).first)[1]);
                double angle = 90. - abs(90. - VectorUtil::getAngle(vector1, vector2));
                if (angle < minAngle) {
                    minAngle = angle;
                }
            }

            double room = (180. - minAngle) / 90. * this->coefficient1 * this->radius + this->coefficient2;

            auto tube = vtkSmartPointer<Tube>::New();

            auto lineCut = LineUtil::cut(lines.at(info.at(j).first).at(0), lines.at(info.at(j).first).at(1), point,
                                         room);
            tube->setStPoint(lineCut.at(1));
            tube->setEndPoint(lineCut.at(3));

            info.at(j).second = room;
            intersections.at(i)->addTube(tube);
        }
    }

    // cut the lines
    for (int i = 0; i < lines.size(); i++) {
        vector<array<double, 3>> points;
        vector<double> rooms;
        for (int j = 0; j < this->intersections.size(); j++) {
            if (this->intersections[j]->hasId(i)) {
                points.emplace_back(this->intersections.at(j)->getPoint());
                rooms.push_back(this->intersections.at(j)->getRoom(i));
            }
        }
        auto linesCut = LineUtil::cut(lines.at(i).at(0), lines.at(i).at(1), points, rooms);
        this->lines.insert(this->lines.end(), linesCut.begin(), linesCut.end());
    }


}


void Graph::update() {

    for (int i = 0; i < this->intersections.size(); i++) {
        auto tubes = this->intersections.at(i)->getTubes();

        auto cylinderUnion = vtkSmartPointer<vtkImplicitBoolean>::New();
        cylinderUnion->SetOperationTypeToUnion();

        for (int j = 0; j < tubes.size(); j++) {
            //todo:增长连接处圆管的长度，阻止间隙的产生
            auto tube = TubeUtil::createTube(tubes.at(j)->getStPoint(), tubes.at(j)->getEndPoint(), this->radius);
            cylinderUnion->AddFunction(tube);
        }

        auto center = this->intersections.at(i)->getPoint();
        auto sample = vtkSmartPointer<vtkSampleFunction>::New();
        sample->SetImplicitFunction(cylinderUnion);
        sample->SetModelBounds(center[0] - 20, center[0] + 20, center[1] - 20, center[1] + 20, center[2] - 20,
                               center[2] + 20);
        sample->SetSampleDimensions(128, 128, 128);
        sample->ComputeNormalsOff();
        auto surface = vtkSmartPointer<vtkContourFilter>::New();
        surface->SetInputConnection(sample->GetOutputPort());
        surface->SetValue(0, 0.0);
        surface->Update();
        this->dataList.emplace_back(surface->GetOutput());
    }
    this->dataList.emplace_back(TubeUtil::createTube(this->lines, this->radius, this->coefficient3));

}


vector<array<array<double, 3>, 2>> &Graph::getLines() {
    return lines;
}

void Graph::setRadius(double radius) {
    Graph::radius = radius;
}

void Graph::setLength(double length) {
    Graph::length = length;
}

vector<vtkSmartPointer<Intersection>> &Graph::getIntersections() {
    return intersections;
}

void Graph::setCoefficient1(double coefficient1) {
    Graph::coefficient1 = coefficient1;
}

void Graph::setCoefficient2(double coefficient2) {
    Graph::coefficient2 = coefficient2;
}

vector<vtkSmartPointer<vtkPolyData>> Graph::getOutput() {
    return dataList;
}

void Graph::setCoefficient3(int coefficient3) {
    Graph::coefficient3 = coefficient3;
}