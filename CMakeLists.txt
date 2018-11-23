cmake_minimum_required(VERSION 3.12)
project(pipeStitching)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

aux_source_directory(src SOURCE_FILES)
set(HEAD_FILES include/STLRender.h include/Cylinder.h include/CellPickerInteractorStyle.h)
set(TEST_FILES test/STLRenderTest.cpp test/CylinderTest.cpp test/CellPickerInteractorStyleTest.cpp)

#vtk
# the root to find Config
#set(VTK_ROOT your_vtk_path)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

#gtest
enable_testing()
# the root to find Config
set(GTEST_ROOT /opt/GTest)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEAD_FILES} ${TEST_FILES})
target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES} GTest::Main)
