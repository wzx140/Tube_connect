cmake_minimum_required(VERSION 3.12)
project(tube_connect)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

aux_source_directory(src SOURCE_FILES)
set(HEAD_FILES include/STLRender.h include/Tube.h include/CellPickerInteractorStyle.h util/VectorUtil.h util/LineUtil.h util/TubeUtil.h util/CircleUtil.h include/Graph.h include/Intersection.h)

# vtk
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_subdirectory(test)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEAD_FILES})
target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})