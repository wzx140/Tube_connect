//
// Created by wzx on 18-11-30.
//

#include <gtest/gtest.h>

#include "../../util/VectorUtil.h"

using namespace VectorUtil;

TEST(VectorUtilTest, getAngleTest) {
    double vector1[3] = {0, 0, 1};
    double vector2[3] = {0, 1, 0};
    double vector3[3] = {0, 0, -1};
    double vector4[3] = {0, 1, 1};

    EXPECT_EQ(getAngle(vector1, vector2), 90);
    EXPECT_EQ(getAngle(vector1, vector3), 180);
    EXPECT_EQ(getAngle(vector1, vector4), 45);
}
