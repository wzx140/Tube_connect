add_subdirectory(gtest)

set(TEST_FILES basic/CellPickerInteractorStyleTest.cpp basic/STLRenderTest.cpp basic/TubeTest.cpp main.cpp util/VectorUtilTest.cpp util/LineUtilTest.cpp basic/GraphTest.cpp)
set(SRC_FILES ../src/CellPickerInteractorStyle.cpp ../src/STLRender.cpp ../src/Tube.cpp ../include/CellPickerInteractorStyle.h ../include/STLRender.h ../include/Tube.h ../util/VectorUtil.h ../src/Graph.cpp ../include/Graph.h ../src/Intersection.cpp ../include/Intersection.h)
set(RESOURCE_FILES res/test1.stl res/test2.stl res/test3.stl res/100.stl)

file(COPY ${RESOURCE_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/res)

add_executable(${PROJECT_NAME}_test ${TEST_FILES} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME}_test ${VTK_LIBRARIES} gtest_main vtkbool)
