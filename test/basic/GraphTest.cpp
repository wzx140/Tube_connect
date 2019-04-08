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

TEST_F(GraphTest, updateTest) {
    this->stlRender->setPath("res/100.stl");
    stlRender->load();
    auto tubes = this->stlRender->getTubes();
    graph->setLength(250);
    graph->setRadius(3);
    graph->setCoefficient3(30);
    graph->create(tubes);
    graph->update();
    stlRender->setInputData(graph->getOutput(), 1);
    stlRender->axisOn();
    stlRender->start();
}