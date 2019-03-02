cmake_minimum_required(VERSION 3.12)
project(tube_connect)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

option(BUILD_TEST "whether to build test" OFF)

aux_source_directory(src SOURCE_FILES)
set(HEAD_FILES include/STLRender.h include/Tube.h include/CellPickerInteractorStyle.h util/VectorUtil.h util/LineUtil.h util/TubeUtil.h util/CircleUtil.h include/Graph.h include/Intersection.h)
set(UI_FILES ui/MainWindow.ui ui/MainWindow.cpp ui/MainWindow.h ui/ui_MainWindow.h)

# vtk
find_package(VTK COMPONENTS
        vtkCommonComputationalGeometry
        vtkCommonCore
        vtkCommonDataModel
        vtkCommonExecutionModel
        vtkCommonMath
        vtkCommonTransforms
        vtkFiltersCore
        vtkFiltersExtraction
        vtkFiltersGeneral
        vtkFiltersModeling
        vtkFiltersSources
        vtkGUISupportQt
        vtkIOGeometry
        vtkIOLegacy
        vtkImagingHybrid
        vtkInteractionStyle
        vtkRenderingAnnotation
        vtkRenderingCore
        vtkRenderingFreeType
        vtkRenderingOpenGL2 REQUIRED)
include(${VTK_USE_FILE})

# qt
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5Widgets REQUIRED)

# vtkbool
include_directories(vtkbool-2.5.2)
add_subdirectory(vtkbool-2.5.2)
include_directories(vtkbool-2.5.2/libs/vp)

if (${BUILD_TEST})
    message("Build test")
    add_subdirectory(test)
endif ()


add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEAD_FILES} ${UI_FILES})
target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES} Qt5::Widgets vtkbool)