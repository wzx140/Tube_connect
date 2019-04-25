//
// Created by wzx on 19-1-11.
//

#include <cmath>

#include "../util/TubeUtil.h"
#include "../include/Graph.h"
#include "../include/STLRender.h"
#include <vtkContourFilter.h>
#include <vtkSampleFunction.h>
#include <vtkAppendPolyData.h>

using std::pair;
using std::sort;


Graph::Graph() {
    this->length = 100;
    this->coefficient = 20;
}

Graph *Graph::New() {
    return new Graph;
}

void Graph::create(vector<vtkSmartPointer<Tube>> tubes) {

    for (const auto &tube : tubes) {
        auto line = tube->getStructureLine();
        auto tubeLine = vtkSmartPointer<Tube>::New();
        tubeLine->setRadius(tube->getRadius());
        tubeLine->setCenterLine(line);
        this->tubeLines.emplace_back(tubeLine);
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
    for (int i = 0; i < this->tubeLines.size(); i++) {
        auto line = LineUtil::extend(this->tubeLines.at(i)->getCenterLine(), this->length);
        this->tubeLines.at(i)->setCenterLine(line);
    }
}


void Graph::update() {
    auto cylinderUnion = vtkSmartPointer<vtkImplicitBoolean>::New();
    cylinderUnion->SetOperationTypeToUnion();

    vector<vtkSmartPointer<vtkImplicitFunction>> tubesRemove;

    double max_x = std::numeric_limits<double>::min();
    double max_y = std::numeric_limits<double>::min();
    double max_z = std::numeric_limits<double>::min();
    double min_x = std::numeric_limits<double>::max();
    double min_y = std::numeric_limits<double>::max();
    double min_z = std::numeric_limits<double>::max();

    for (auto &tubeLine : this->tubeLines) {
        auto radius = tubeLine->getRadius();
        auto line = tubeLine->getCenterLine();
        tubeLine->setMode(1);
        tubeLine->update();
        auto bounds = tubeLine->getBounds();

//      get bounds
        if (bounds[0] < min_x) {
            min_x = bounds[0];
        }
        if (bounds[1] > max_x) {
            max_x = bounds[1];
        }
        if (bounds[2] < min_y) {
            min_y = bounds[2];
        }
        if (bounds[3] > max_y) {
            max_y = bounds[3];
        }
        if (bounds[4] < min_z) {
            min_z = bounds[4];
        }
        if (bounds[5] > max_z) {
            max_z = bounds[5];
        }

        auto extendLine1 = LineUtil::extend(line, 2);
        auto extendLine2 = LineUtil::extend(line, -2);

//            located the caps on the tube
        auto tubeRemove1 = TubeUtil::createTube(extendLine1[0], extendLine2[0], radius + 0.2);
        auto tubeRemove2 = TubeUtil::createTube(extendLine1[1], extendLine2[1], radius + 0.2);
        tubesRemove.emplace_back(tubeRemove1);
        tubesRemove.emplace_back(tubeRemove2);

        auto tube = TubeUtil::createTube(line[0], line[1], radius);
        cylinderUnion->AddFunction(tube);
    }


    auto sample = vtkSmartPointer<vtkSampleFunction>::New();
    sample->SetImplicitFunction(cylinderUnion);
    sample->SetModelBounds(min_x - 1,
                           max_x + 1,
                           min_y - 1,
                           max_y + 1,
                           min_z - 1,
                           max_z + 1);
    int param1 = int(this->coefficient * ((max_x - min_x + 2) / 10));
    int param2 = int(this->coefficient * ((max_y - min_y + 2) / 10));
    int param3 = int(this->coefficient * ((max_z - min_z + 2) / 10));
    sample->SetSampleDimensions(param1, param2, param3);
    sample->ComputeNormalsOff();
    auto surface = vtkSmartPointer<vtkContourFilter>::New();
    surface->SetInputConnection(sample->GetOutputPort());
    surface->SetValue(0, 0.0);
    surface->Update();

//        remove caps on the tube
    auto data = TubeUtil::clip(surface->GetOutput(), tubesRemove);

    this->dataList.emplace_back(data);
}

void Graph::setLength(double length) {
    Graph::length = length;
}

void Graph::setCoefficient(double coefficient) {
    Graph::coefficient = coefficient;
}

vector<vtkSmartPointer<vtkPolyData>> Graph::getOutput() {
    return dataList;
}