//
// Created by wzx on 18-11-21.
//
#include <gtest/gtest.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkAxesActor.h>
#include <vector>

#include "../../include/STLRender.h"

using std::vector;

class STLRenderTest : public ::testing::Test {

protected:
    vtkSmartPointer<STLRender> stlRender;

    virtual void SetUp() {
        stlRender = vtkSmartPointer<STLRender>::New();
    }
};

/**
 * if you see a tube, the test is pass
 */
TEST_F(STLRenderTest, test1) {
    stlRender->setPath("res/test1.stl");
    stlRender->load();
    stlRender->setInputData(stlRender->getData());
    stlRender->start();
}

/**
 * if you see three tubes, the test is pass
 */
TEST_F(STLRenderTest, test2) {
    stlRender->setPath("res/test2.stl");
    stlRender->load();
    stlRender->setInputData(stlRender->getData());
    stlRender->start();
}

/**
 * if you see a tube with axis, the test is pass
 */
TEST_F(STLRenderTest, testAxisOn) {
    stlRender->setPath("res/test1.stl");
    stlRender->load();
    stlRender->setInputData(stlRender->getData());
    stlRender->axisOn();
    stlRender->start();
}


/**
 * if you see four renderers in a render window with three, one, one, one tube in turn, the test is pass
 */
TEST_F(STLRenderTest, getCylinderstest) {

//    Define viewport ranges
    double viewport1[4] = {0.0, 0.0, 0.25, 1.0};
    double viewport2[4] = {0.25, 0.0, 0.5, 1.0};
    double viewport3[4] = {0.5, 0.0, 0.75, 1.0};
    double viewport4[4] = {0.75, 0.0, 1.0, 1.0};

    stlRender->setPath("res/test2.stl");
    stlRender->load();
    auto tubes = stlRender->getTubes();

    EXPECT_EQ(tubes.size(), 3);

    auto axes = vtkSmartPointer<vtkAxesActor>::New();
    axes->SetShaftType(0);
    axes->SetTotalLength(10, 10, 10);
    axes->SetAxisLabels(0);


    auto mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper1->SetInputData(stlRender->getData());
    mapper1->Update();
    auto actor1 = vtkSmartPointer<vtkActor>::New();
    actor1->SetMapper(mapper1);
    auto renderer1 = vtkSmartPointer<vtkRenderer>::New();
    renderer1->AddActor(actor1);
    renderer1->SetViewport(viewport1);
    renderer1->AddActor(axes);

    auto mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper2->SetInputData(tubes.at(0)->getData());
    mapper2->Update();
    auto actor2 = vtkSmartPointer<vtkActor>::New();
    actor2->SetMapper(mapper2);
    auto renderer2 = vtkSmartPointer<vtkRenderer>::New();
    renderer2->AddActor(actor2);
    renderer2->SetViewport(viewport2);
    renderer2->AddActor(axes);

    auto mapper3 = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper3->SetInputData(tubes.at(1)->getData());
    mapper3->Update();
    auto actor3 = vtkSmartPointer<vtkActor>::New();
    actor3->SetMapper(mapper3);
    auto renderer3 = vtkSmartPointer<vtkRenderer>::New();
    renderer3->AddActor(actor3);
    renderer3->SetViewport(viewport3);
    renderer3->AddActor(axes);

    auto mapper4 = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper4->SetInputData(tubes.at(2)->getData());
    mapper4->Update();
    auto actor4 = vtkSmartPointer<vtkActor>::New();
    actor4->SetMapper(mapper4);
    auto renderer4 = vtkSmartPointer<vtkRenderer>::New();
    renderer4->AddActor(actor4);
    renderer4->SetViewport(viewport4);
    renderer4->AddActor(axes);

    auto window = vtkSmartPointer<vtkRenderWindow>::New();
    window->AddRenderer(renderer1);
    window->AddRenderer(renderer2);
    window->AddRenderer(renderer3);
    window->AddRenderer(renderer4);

    auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(window);
    interactor->SetInteractorStyle(vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New());

    interactor->Initialize();
    interactor->Start();

}