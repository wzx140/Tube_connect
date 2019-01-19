//
// Created by wzx on 18-12-3.
//
#include <gtest/gtest.h>
#include <vtkSphereSource.h>
#include "../../include/STLRender.h"
#include "../../util/TubeUtil.h"
#include "../../include/Graph.h"

using std::vector;

class TubeUtilTest : public ::testing::Test {

protected:

    vtkSmartPointer<STLRender> stlRender;

    virtual void SetUp() {
        stlRender = vtkSmartPointer<STLRender>::New();
        stlRender->setPath("res/test2.stl");
        stlRender->load();
    }
};

TEST_F(TubeUtilTest, connectTest) {
    auto tubes = stlRender->getTubes();

    auto graph = vtkSmartPointer<Graph>::New();
    graph->setLength(200);
    graph->setRadius(5);
    graph->setCoefficient3(0.05);
    graph->create(tubes);
    auto interSection = graph->getIntersections()[0];
    auto tube1 = interSection->getTubes()[0];
    auto tube2 = interSection->getTubes()[2];

    vector<vtkSmartPointer<vtkPolyData>> dataList;
    auto edge1 = tube1->getEdgePoints();
    auto edge2 = tube2->getEdgePoints();

    array<double, 3> center{0, 0, 0};

    auto data = TubeUtil::connect(edge1, tube1->getNormal(), edge2, tube2->getNormal(), center, 2, 5);
    dataList.insert(dataList.end(), data.begin(), data.end());

    for (int i = 0; i < edge1.size(); i++) {
        auto sphere = vtkSmartPointer<vtkSphereSource>::New();
        sphere->SetCenter(edge1[i].data());
        sphere->SetRadius(0.2);
        sphere->Update();
        dataList.emplace_back(sphere->GetOutput());
    }
    for (int i = 0; i < edge2.size(); i++) {
        auto sphere = vtkSmartPointer<vtkSphereSource>::New();
        sphere->SetCenter(edge2[i].data());
        sphere->SetRadius(0.2);
        sphere->Update();
        dataList.emplace_back(sphere->GetOutput());
    }

    this->stlRender->setInputData(dataList, 1);
    this->stlRender->axisOn();
    this->stlRender->start();
}

TEST_F(TubeUtilTest, createTubeTest) {
    array<array<double, 3>, 2> line1 = {{{0, 0, 0}, {10, 0, 0}}};
    array<array<double, 3>, 2> line2 = {{{0, 0, 0}, {0, 10, 0}}};
    vector<array<array<double, 3>, 2>> lines;
    lines.emplace_back(line1);
    lines.emplace_back(line2);
    auto graph = TubeUtil::createTube(lines, 2, 20);


    this->stlRender->setInputData(graph, 1);
    this->stlRender->axisOn();
    this->stlRender->start();
}
