cmake_minimum_required(VERSION 3.12)
project(pipeStitching)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

aux_source_directory(src SOURCE_FILES)
set(HEAD_FILES include/STLRender.h include/Cylinder.h include/CellPickerInteractorStyle.h)
set(TEST_FILES test/STLRenderTest.cpp test/CylinderTest.cpp test/CellPickerInteractorStyleTest.cpp test/main.cpp)

#vtk
# the root to find Config
#set(VTK_ROOT your_vtk_path)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif ()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif ()


add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEAD_FILES})
target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})

add_executable(${PROJECT_NAME}_test ${SOURCE_FILES} ${HEAD_FILES} ${TEST_FILES})
target_link_libraries(${PROJECT_NAME}_test ${VTK_LIBRARIES} gtest_main)
