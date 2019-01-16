//
// Created by wzx on 19-1-11.
//

#include "../include/Graph.h"
#include "../util/LineUtil.h"

Graph::Graph() {
    this->length = 100;
    this->radius = 3;
}

Graph *Graph::New() {
    return new Graph;
}

void Graph::create(vector<vtkSmartPointer<Tube>> tubes) {
    vector<array<array<double, 3>, 2>> lines{};

    for (const auto &tube : tubes) {
        lines.emplace_back(tube->getStructureLine());
    }

    // extend the lines
    for (int i = 0; i < lines.size(); i++) {
        LineUtil::extend(lines.at(i)[0], lines.at(i)[1], this->length);
        this->lines.emplace_back(lines.at(i));
    }

//    // get intersections
//    array<double, 3> point{};
//    for (int i = 0; i < lines.size(); i++) {
//        for (int j = i + 1; j < lines.size(); j++) {
//
//            if (LineUtil::intersection3D(lines[i][0], lines[i][1], lines[j][0], lines[j][1], point)) {
//
//                int flag = 0;
//                for (int k = 0; k < this->intersections.size(); k++) {
//                    if (this->intersections[k]->hasPoint(point)) {
//                        flag = 1;
//
//                        if (this->intersections[k]->hasId(i) && !this->intersections[k]->hasId(j)) {
//                            auto tube1 = vtkSmartPointer<Tube>::New();
//                            auto tube2 = vtkSmartPointer<Tube>::New();
//                            this->intersections[k]->addId(j);
//                            auto lineCut = LineUtil::cut(lines.at(j).at(0), lines.at(j).at(1), point, this->radius* 2);
//                            auto normal1 = VectorUtil::getVector(lineCut.at(1), point);
//                            auto normal2 = VectorUtil::getVector(lineCut.at(3), point);
//                            tube1->update(normal1, lineCut.at(1), this->radius);
//                            tube2->update(normal2, lineCut.at(3), this->radius);
//                            this->intersections[k]->addTube(tube1);
//                            this->intersections[k]->addTube(tube2);
//
//
//                        } else if (this->intersections[k]->hasId(j) && !this->intersections[k]->hasId(i)) {
//                            auto tube1 = vtkSmartPointer<Tube>::New();
//                            auto tube2 = vtkSmartPointer<Tube>::New();
//                            this->intersections[k]->addId(i);
//                            auto lineCut = LineUtil::cut(lines.at(i).at(0), lines.at(i).at(1), point, this->radius* 2);
//                            auto normal1 = VectorUtil::getVector(lineCut.at(1), point);
//                            auto normal2 = VectorUtil::getVector(lineCut.at(3), point);
//                            tube1->update(normal1, lineCut.at(1), this->radius);
//                            tube2->update(normal2, lineCut.at(3), this->radius);
//                            this->intersections[k]->addTube(tube1);
//                            this->intersections[k]->addTube(tube2);
//
//                        }
//                        break;
//                    }
//                }
//                if (flag == 0) {
//                    auto intersection = vtkSmartPointer<Intersection>::New();
//                    intersection->addId(i);
//                    intersection->addId(j);
//
//                    auto tube1 = vtkSmartPointer<Tube>::New();
//                    auto tube2 = vtkSmartPointer<Tube>::New();
//                    auto tube3 = vtkSmartPointer<Tube>::New();
//                    auto tube4 = vtkSmartPointer<Tube>::New();
//
//                    auto lineCut1 = LineUtil::cut(lines.at(i).at(0), lines.at(i).at(1), point, this->radius* 2);
//                    auto normal11 = VectorUtil::getVector(lineCut1.at(1), point);
//                    auto normal12 = VectorUtil::getVector(lineCut1.at(3), point);
//                    tube1->update(normal11, lineCut1.at(1), this->radius);
//                    tube2->update(normal12, lineCut1.at(3), this->radius);
//
//                    auto lineCut2 = LineUtil::cut(lines.at(j).at(0), lines.at(j).at(1), point, this->radius* 2);
//                    auto normal21 = VectorUtil::getVector(lineCut2.at(1), point);
//                    auto normal22 = VectorUtil::getVector(lineCut2.at(3), point);
//                    tube3->update(normal21, lineCut2.at(1), this->radius);
//                    tube4->update(normal22, lineCut2.at(3), this->radius);
//
//                    intersection->setPoint(point[0], point[1], point[2]);
//                    intersection->addTube(tube1);
//                    intersection->addTube(tube2);
//                    intersection->addTube(tube3);
//                    intersection->addTube(tube4);
//
//                    this->intersections.emplace_back(intersection);
//                }
//            }
//        }
//    }
//
////    cut the lines
//    for (int i = 0; i < lines.size(); i++) {
//        vector<array<double, 3>> points;
//        for (int j = 0; j < this->intersections.size(); j++) {
//            if (this->intersections[j]->hasId(i)) {
//                points.emplace_back(this->intersections[j]->getPoint());
//            }
//        }
//        auto linesCut = LineUtil::cut(lines.at(i).at(0), lines.at(i).at(1), points, this->radius * 2);
//        this->lines.insert(this->lines.end(), linesCut.begin(), linesCut.end());
//    }

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
