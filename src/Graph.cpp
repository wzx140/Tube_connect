//
// Created by wzx on 19-1-11.
//

#include <cmath>

#include "../util/TubeUtil.h"
#include "../include/Graph.h"

using std::pair;
using std::sort;

Graph::Graph() {
    this->length = 100;
    this->radius = 3;
    this->coefficient1 = 1.5;
    this->coefficient2 = 2;
    this->coefficient3 = 0.2;
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
                if (length2 < min && length2 <= this->length) {
                    min = length2;
                    stPoint = lines.at(i)[1];
                    endPoint = lines.at(j)[0];
                } else if (length3 < min && length3 <= this->length) {
                    min = length3;
                    stPoint = lines.at(i)[0];
                    endPoint = lines.at(j)[1];
                } else if (length4 < min && length4 <= this->length) {
                    min = length4;
                    stPoint = lines.at(i)[0];
                    endPoint = lines.at(j)[0];
                }

                lines.erase(lines.begin() + i);
                lines.erase(lines.begin() + j - 1);

                array<array<double, 3>, 2> line{stPoint, endPoint};
                lines.emplace_back(line);
                i--;
                break;
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

            double minAngle = 90;
            auto vector1 = VectorUtil::getVector(lines.at(info.at(j).first)[0], lines.at(info.at(j).first)[1]);
            for (int k = 0; k < info.size(); k++) {
                if (k == j) { continue; }
                auto vector2 = VectorUtil::getVector(lines.at(info.at(k).first)[0], lines.at(info.at(k).first)[1]);
                double angle = 90 - abs(90 - VectorUtil::getAngle(vector1, vector2));
                if (angle < minAngle) {
                    minAngle = angle;
                }
            }

            double room = pow((180 - minAngle) / 90, this->coefficient2) * this->radius * this->coefficient1;

            auto tube1 = vtkSmartPointer<Tube>::New();
            auto tube2 = vtkSmartPointer<Tube>::New();
            int resolution = static_cast<int>(180 * (info.size() - 1) * this->coefficient3) + 2;
            tube1->setResolution(resolution);
            tube2->setResolution(resolution);

            auto lineCut = LineUtil::cut(lines.at(info.at(j).first).at(0), lines.at(info.at(j).first).at(1), point,
                                         room);
            auto normal1 = VectorUtil::getVector(lineCut.at(1), point);
            auto normal2 = VectorUtil::getVector(lineCut.at(3), point);
            tube1->update(normal1, lineCut.at(1), this->radius);
            tube2->update(normal2, lineCut.at(3), this->radius);

            info.at(j).second = room;
            intersections.at(i)->addTube(tube1);
            intersections.at(i)->addTube(tube2);
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

bool cmp(pair<array<int, 2>, double> &T1, pair<array<int, 2>, double> &T2);

void Graph::update() {
    this->dataList.emplace_back(TubeUtil::createTube(this->lines, this->radius, 20));

    for (int i = 0; i < this->intersections.size(); i++) {
        //sort by angle
        auto tubes = this->intersections.at(i)->getTubes();
        vector<pair<array<int, 2>, double>> tubePairs;
        for (int j = 0; j < tubes.size(); j++) {
            for (int k = j + 1; k < tubes.size(); k++) {
                auto angle = VectorUtil::getAngle(tubes.at(j)->getNormal(), tubes.at(k)->getNormal());
                tubePairs.emplace_back(pair<array<int, 2>, double>(array<int, 2>{j, k}, angle));
            }
        }
        sort(tubePairs.begin(), tubePairs.end(), cmp);

        //connect
        for (int i = 0; i < tubePairs.size(); i++) {

            double angle = tubePairs.at(i).second;
            if (angle == 180) {
                continue;
            }
            int id1 = tubePairs.at(i).first[0];
            int id2 = tubePairs.at(i).first[1];
            int resolution = static_cast<int>((180 - angle) * this->coefficient3 / 2);
            auto data = TubeUtil::connect(tubes.at(id1)->getEdgePoints(), tubes.at(id1)->getNormal(),
                                          tubes.at(id2)->getEdgePoints(), tubes.at(id2)->getNormal(), resolution, 50);
            this->dataList.insert(dataList.end(), data.begin(), data.end());
        }

    }


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

vector<vtkSmartPointer<vtkPolyData>> Graph::getOutput(int i) {

    vector<vtkSmartPointer<vtkPolyData>> data;
    if (i == 0) {
        return dataList;

    } else if (i == 1) {
        for (int i = 1; i < this->dataList.size(); i++) {
            data.emplace_back(dataList.at(i));
        }
        return data;
    } else if (i == 2) {
        data.emplace_back(dataList.at(0));
        return data;
    }
    return dataList;

}

void Graph::setCoefficient3(double coefficient3) {
    Graph::coefficient3 = coefficient3;
}

/**
 * used to sort
 * @return
 */
bool cmp(pair<array<int, 2>, double> &T1, pair<array<int, 2>, double> &T2) {
    double angle1 = T1.second;
    double angle2 = T2.second;
    return angle1 < angle2;
}