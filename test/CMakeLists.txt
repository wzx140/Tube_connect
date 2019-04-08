add_subdirectory(gtest)

set(TEST_FILES basic/STLRenderTest.cpp basic/TubeTest.cpp main.cpp util/VectorUtilTest.cpp util/LineUtilTest.cpp basic/GraphTest.cpp)
set(SRC_FILES ../src/STLRender.cpp ../src/Tube.cpp ../include/STLRender.h ../include/Tube.h ../util/VectorUtil.h ../src/Graph.cpp ../include/Graph.h ../src/Intersection.cpp ../include/Intersection.h util/TubeUtilTest.cpp)

add_executable(${PROJECT_NAME}_test ${TEST_FILES} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME}_test ${VTK_LIBRARIES} gtest_main)
