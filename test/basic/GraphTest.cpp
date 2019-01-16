//
// Created by wzx on 19-1-12.
//

#include <gtest/gtest.h>
#include <vtkLine.h>
#include <vtkSphereSource.h>

#include "../../include/STLRender.h"
#include "../../include/Graph.h"
#include "../../util/TubeUtil.h"

class GraphTest : public ::testing::Test {

protected:
    vtkSmartPointer<STLRender> stlRender;
    vtkSmartPointer<Graph> graph;

    virtual void SetUp() {
        this->stlRender = vtkSmartPointer<STLRender>::New();
        this->graph = vtkSmartPointer<Graph>::New();
    }
};

TEST_F(GraphTest, createTest) {
    this->stlRender->setPath("res/test3.stl");
    stlRender->load();
    auto tubes = this->stlRender->getTubes();
    graph->setLength(200);
    graph->setRadius(10);
    graph->create(tubes);
    auto lines = graph->getLines();
    auto data = TubeUtil::createTube(lines, 5, 20);
    stlRender->setInputData(data, 1);
    stlRender->start();

//    auto data = vtkSmartPointer<vtkPolyData>::New();
//    auto points = vtkSmartPointer<vtkPoints>::New();
//    auto lines_ = vtkSmartPointer<vtkCellArray>::New();
//    vector<vtkSmartPointer<vtkPolyData>> dataList;
//
//    for (int i = 0; i < lines.size(); i++) {
//        auto line = vtkSmartPointer<vtkLine>::New();
//
//        points->InsertNextPoint(lines.at(i).at(0).data());
//        points->InsertNextPoint(lines.at(i).at(1).data());
//
//        line->GetPointIds()->SetId(0, i * 2);
//        line->GetPointIds()->SetId(1, i * 2 + 1);
//
//        lines_->InsertNextCell(line);
//    }
//
//    data->SetLines(lines_);
//    data->SetPoints(points);
//    dataList.emplace_back(data);

//    auto source = this->graph->getIntersections();
//    for (int i = 0; i < source.size(); i++) {
//        auto point = source[i]->getPoint();
//        auto sphere = vtkSmartPointer<vtkSphereSource>::New();
//        sphere->SetCenter(point.data());
//        sphere->SetRadius(2);
//        sphere->Update();
//        dataList.emplace_back(sphere->GetOutput());
//
//        auto tubes = source[i]->getTubes();
//
//        for (int j = 0; j < tubes.size(); j++) {
//            auto points = tubes.at(j)->getEdgePoints();
//            for (int k = 0; k < points.size(); k++) {
//                auto sphere = vtkSmartPointer<vtkSphereSource>::New();
//                sphere->SetCenter(points.at(k).data());
//                sphere->SetRadius(0.5);
//                sphere->Update();
//                dataList.emplace_back(sphere->GetOutput());
//            }
//        }
//    }

//    stlRender->setInputData(dataList, 1);
//    stlRender->axisOn();
//    stlRender->start();

}