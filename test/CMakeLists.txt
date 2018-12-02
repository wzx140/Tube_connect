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

set(TEST_FILES basic/CellPickerInteractorStyleTest.cpp basic/STLRenderTest.cpp basic/TubeTest.cpp main.cpp util/VectorUtilTest.cpp util/LineUtilTest.cpp)
set(SRC_FILES ../src/CellPickerInteractorStyle.cpp ../src/STLRender.cpp ../src/Tube.cpp ../include/CellPickerInteractorStyle.h ../include/STLRender.h ../include/Tube.h ../util/VectorUtil.h)
set(RESOURCE_FILES res/test1.stl res/test2.stl)

file(COPY ${RESOURCE_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/res)

add_executable(${PROJECT_NAME}_test ${TEST_FILES} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME}_test ${VTK_LIBRARIES} gtest_main)
