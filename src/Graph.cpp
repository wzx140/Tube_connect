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


Graph::Graph() {
    this->length = 100;
    this->coefficient1 = 1.2;
    this->coefficient2 = 20;
    this->coefficient3 = 30;
}

Graph *Graph::New() {
    return new Graph;
}

void Graph::create(vector<vtkSmartPointer<Tube>> tubes) {

//    unlike this->tubeLines, this only store the original line
    vector<vtkSmartPointer<Tube>> tubeLines;

    for (const auto &tube : tubes) {
        auto line = tube->getStructureLine();
        auto tubeLine = vtkSmartPointer<Tube>::New();
        tubeLine->setRadius(tube->getRadius());
        tubeLine->setCenterLine(line);
        tubeLines.emplace_back(tubeLine);
    }

    // combine the same direction line
    for (int i = 0; i < tubeLines.size(); i++) {
        for (int j = i + 1; j < tubeLines.size(); j++) {
            auto line1 = tubeLines.at(i)->getCenterLine();
            auto line2 = tubeLines.at(j)->getCenterLine();
            double radius1 = tubeLines.at(i)->getRadius();
            double radius2 = tubeLines.at(j)->getRadius();

            if (LineUtil::isCollinear(line1, line2) && abs(radius1 - radius2) < 0.01) {
                double length1 = LineUtil::getLength(line1[0], line2[0]);
                double length2 = LineUtil::getLength(line1[0], line2[1]);
                double length3 = LineUtil::getLength(line1[1], line2[0]);
                double length4 = LineUtil::getLength(line1[1], line2[1]);

                // find minimum
                double min = length1;
                array<double, 3> stPoint = line1[1];
                array<double, 3> endPoint = line2[1];
                if (length2 < min) {
                    min = length2;
                    stPoint = line1[1];
                    endPoint = line2[0];
                }
                if (length3 < min) {
                    min = length3;
                    stPoint = line1[0];
                    endPoint = line2[1];
                }
                if (length4 < min) {
                    min = length4;
                    stPoint = line1[0];
                    endPoint = line2[0];
                }

                if (min <= this->length) {
                    tubeLines.erase(tubeLines.begin() + i);
                    tubeLines.erase(tubeLines.begin() + j - 1);

                    auto tube = vtkSmartPointer<Tube>::New();
                    array<array<double, 3>, 2> line{stPoint, endPoint};
                    tube->setCenterLine(line);
                    tube->setRadius(radius1);
                    tubeLines.emplace_back(tube);
                    i--;
                    break;
                }
            }
        }
    }

    // extend the lines
    for (int i = 0; i < tubeLines.size(); i++) {
        auto line = LineUtil::extend(tubeLines.at(i)->getCenterLine(), this->length);
        tubeLines.at(i)->setCenterLine(line);
    }

//    for (auto &item : tubeLines) {
//        auto tube = vtkSmartPointer<Tube>::New();
//        tube->setRadius(item->getRadius());
//        tube->setCenterLine(item->getCenterLine());
//        this->tubeLines.emplace_back(tube);
//    }

    // get cross points
    array<double, 3> point{};
    for (int i = 0; i < tubeLines.size(); i++) {
        for (int j = i + 1; j < tubeLines.size(); j++) {
            auto line1 = tubeLines.at(i)->getCenterLine();
            auto line2 = tubeLines.at(j)->getCenterLine();
            if (LineUtil::intersection3D(line1[0], line1[1], line2[0], line2[1], point)) {

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

            auto tube1 = tubeLines.at(info.at(j).first);
            auto line1 = tube1->getCenterLine();
            double minAngle = 90.;
            double radius1 = tube1->getRadius();
            double radiusMax = radius1;
            auto vector1 = VectorUtil::getVector(line1[0], line1[1]);
            for (int k = 0; k < info.size(); k++) {
                if (k == j) { continue; }
                auto tube2 = tubeLines.at(info.at(k).first);
                auto line2 = tube2->getCenterLine();
                auto vector2 = VectorUtil::getVector(line2[0], line2[1]);
                double angle = 90. - abs(90. - VectorUtil::getAngle(vector1, vector2));
                if (angle < minAngle) {
                    minAngle = angle;
                }
                if (radiusMax < tube2->getRadius()) {
                    radiusMax = tube2->getRadius();
                }
            }

            double room = pow((180. - minAngle) / 90., 2) * this->coefficient1 * radiusMax;

            auto tube = vtkSmartPointer<Tube>::New();
            tube->setRadius(radius1);

            auto lineCut = LineUtil::cut(line1.at(0), line1.at(1), point, room);
            array<array<double, 3>, 2> line{lineCut.at(1), lineCut.at(3)};
            tube->setCenterLine(line);

            info.at(j).second = room;
            intersections.at(i)->addTube(tube);
        }
    }

    // cut the lines
    for (int i = 0; i < tubeLines.size(); i++) {
        vector<array<double, 3>> points;
        vector<double> rooms;
        for (int j = 0; j < this->intersections.size(); j++) {
            if (this->intersections[j]->hasId(i)) {
                points.emplace_back(this->intersections.at(j)->getPoint());
                rooms.push_back(this->intersections.at(j)->getRoom(i));
            }
        }
        auto line = tubeLines.at(i)->getCenterLine();
        auto radius = tubeLines.at(i)->getRadius();
        auto linesCut = LineUtil::cut(line.at(0), line.at(1), points, rooms);
        for (auto &item : linesCut) {
            auto tube = vtkSmartPointer<Tube>::New();
            tube->setRadius(radius);
            tube->setCenterLine(item);
            this->tubeLines.emplace_back(tube);
        }

    }


}


void Graph::update() {

    for (int i = 0; i < this->intersections.size(); i++) {
        auto tubes = this->intersections.at(i)->getTubes();

        auto cylinderUnion = vtkSmartPointer<vtkImplicitBoolean>::New();
        cylinderUnion->SetOperationTypeToUnion();

        vector<vtkSmartPointer<vtkImplicitFunction>> tubesRemove;

        for (int j = 0; j < tubes.size(); j++) {
//            extend the tube to avoid the gap
            auto radius = tubes.at(j)->getRadius();
            auto line = tubes.at(j)->getCenterLine();
            double length = LineUtil::getLength(line[0], line[1]);
            auto line2 = LineUtil::extend(line, length / 6);
            auto extendLine1 = LineUtil::extend(line, length / 5);
            auto extendLine2 = LineUtil::extend(line, length / 4);

//            located the caps on the tube
            auto tubeRemove1 = TubeUtil::createTube(line2[0], extendLine2[0], radius + 0.5);
            auto tubeRemove2 = TubeUtil::createTube(line2[1], extendLine2[1], radius + 0.5);
            tubesRemove.emplace_back(tubeRemove1);
            tubesRemove.emplace_back(tubeRemove2);

            auto tube = TubeUtil::createTube(extendLine1[0], extendLine1[1], radius);
            cylinderUnion->AddFunction(tube);
        }

        double maxRoom = 0;
        auto info = intersections.at(i)->getInfo();
        for (auto &item : info) {
            if (item.second > maxRoom) {
                maxRoom = item.second;
            }
        }

        auto center = this->intersections.at(i)->getPoint();
        auto sample = vtkSmartPointer<vtkSampleFunction>::New();
        sample->SetImplicitFunction(cylinderUnion);
        sample->SetModelBounds(center[0] - maxRoom - 3,
                               center[0] + maxRoom + 3,
                               center[1] - maxRoom - 3,
                               center[1] + maxRoom + 3,
                               center[2] - maxRoom - 3,
                               center[2] + maxRoom + 3);
        int param = int(this->coefficient2 * ((2 * maxRoom + 6) / 10));
        sample->SetSampleDimensions(param, param, param);
        sample->ComputeNormalsOff();
        auto surface = vtkSmartPointer<vtkContourFilter>::New();
        surface->SetInputConnection(sample->GetOutputPort());
        surface->SetValue(0, 0.0);
        surface->Update();

//        remove caps on the tube
        auto data = TubeUtil::clip(surface->GetOutput(), tubesRemove);

        this->dataList.emplace_back(data);
    }
    this->dataList.emplace_back(TubeUtil::createTube(this->tubeLines, this->coefficient3));

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