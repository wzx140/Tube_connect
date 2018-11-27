#include <vtkSmartPointer.h>
#include "../include/CellPickerInteractorStyle.h"
#include "../include/STLRender.h"

int main(int argc, char **argv) {
    auto style = vtkSmartPointer<CellPickerInteractorStyle>::New();
    auto stlRender = vtkSmartPointer<STLRender>::New();

    style->SetDefaultRenderer(stlRender->getRenderer());
    stlRender->setStyle(style);

    stlRender->setPath("../test/res/test2.STL");
    stlRender->load();
    stlRender->setInputData(stlRender->getData());
    style->setStlRender(stlRender);
    style->setSelectMode(true);
    stlRender->start();
}