//
// Created by wzx on 18-11-22.
//
#include <gtest/gtest.h>
#include "../include/CellPickerInteractorStyle.h"
#include "../include/STLRender.h"


class CellPickerInteractorStyleTest : public ::testing::Test {

protected:
    vtkSmartPointer<CellPickerInteractorStyle> style;
    vtkSmartPointer<STLRender> stlRender;

    virtual void SetUp() {
        this->stlRender = vtkSmartPointer<STLRender>::New();
        this->style = vtkSmartPointer<CellPickerInteractorStyle>::New();
        this->style->SetDefaultRenderer(this->stlRender->getRenderer());
        this->stlRender->setStyle(this->style);
    }
};

/**
 * if you can select cell in the cylinder, the test is pass
 */
TEST_F(CellPickerInteractorStyleTest, OnLeftButtonDownTest) {
    this->stlRender->setPath("../test/res/test2.STL");
    this->stlRender->load();
    this->stlRender->setInputData(this->stlRender->getData());
    this->style->setStlRender(this->stlRender);
    this->style->setSelectMode(true);
    this->stlRender->start();
}
