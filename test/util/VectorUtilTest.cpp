//
// Created by wzx on 18-11-30.
//

#include <gtest/gtest.h>

#include "../../util/VectorUtil.h"

using namespace VectorUtil;

TEST(VectorUtilTest, getAngleTest) {
    array<double, 3> vector1 = {0, 0, 1};
    array<double, 3> vector2 = {0, 1, 0};
    array<double, 3> vector3 = {0, 0, -1};
    array<double, 3> vector4 = {0, 1, 1};

    EXPECT_EQ(getAngle(vector1, vector2), 90);
    EXPECT_EQ(getAngle(vector1, vector3), 180);
    EXPECT_EQ(getAngle(vector1, vector4), 45);
}
