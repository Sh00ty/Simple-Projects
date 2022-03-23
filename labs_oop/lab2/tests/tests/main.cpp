//
//  test.cpp
//  lab2
//
//  Created by Павел on 17.09.2021.
//


#include <iostream>
#include <gtest/gtest.h>
#include <Curve_Class.hpp>


bool is_equal(double r, double d)
{
    if (fabs(r - d) < std::numeric_limits<double>::epsilon())
        return true;
    return false;
}

TEST(what_type, common_type)
{
    Hc hc1(10, 5.0009, 5.0009, 0, 0);
    ASSERT_EQ(0, hc1.what_type());
    Hc hc2(100, 500.9999, 500.9999, 0, 0);
    ASSERT_EQ(0, hc2.what_type());
    Hc hc3(100, 50, 50, 0, 0);
    ASSERT_EQ(0, hc3.what_type());
}

TEST(what_type, elongated_type)
{
    Hc hc1(10, 5, 5.001, 0, 0);
    ASSERT_EQ(1, hc1.what_type());
    Hc hc2(100, 500000, 5000000.001, 0, 0);
    ASSERT_EQ(1, hc2.what_type());
    Hc hc3(100, 5.9998, 5.9999, 0, 0);
    ASSERT_EQ(1, hc3.what_type());
}


TEST(what_type, shoted_type)
{
    Hc hc1(10, 50, 1.93423, 0, 0);
    ASSERT_EQ(-1, hc1.what_type());
    Hc hc2(100, 500, 49.99, 0, 0);
    ASSERT_EQ(-1, hc2.what_type());
    Hc hc3(100, 1000000.9999, 100000, 0, 0);
    ASSERT_EQ(-1, hc3.what_type());
}


TEST(is_correct, falsee)
{
    Hc hc1(0, 0, 0, 0, 0);
    ASSERT_FALSE(hc1.is_correct());
    Hc hc2(10, 500, 49.99, 0, 0);
    ASSERT_FALSE(hc1.is_correct());
    Hc hc3(100, 10, -1, 0, 0);
    ASSERT_FALSE(hc1.is_correct());
}


TEST(what_type, truee)
{
    Hc hc1(100, 50, 1.93423, 0, 0);
    ASSERT_TRUE(hc1.is_correct());
    Hc hc2(1000, 500, 49.99, 0, 0);
    ASSERT_TRUE(hc1.is_correct());
    Hc hc3(1000000.99999, 1000000.9999, 100000, 0, 0);
    ASSERT_TRUE(hc1.is_correct());
}

TEST(constructor, def)
{
    Hc hc1;
    ASSERT_TRUE(hc1.get_big_radius() == 0 && hc1.get_offset() == 0 && hc1.get_small_radius() == 0);
}


TEST(constructor, init)
{
    Hc hc1(100.0099, 10.001, 1.1, 0, 0);
    double x = 100.0099;
    double y = 10.001;
    double d = 1.1;
    ASSERT_TRUE(is_equal(x, hc1.get_big_radius()) && is_equal(y, hc1.get_small_radius()) && is_equal(d, hc1.get_offset()));
}

TEST(constructor, copy)
{
    Hc hc(100.0099, 10.001, 1.1, 0, 0);
    Hc hc1 = hc;
    double x = 100.0099;
    double y = 10.001;
    double d = 1.1;
    ASSERT_TRUE(is_equal(x, hc1.get_big_radius()) && is_equal(y, hc1.get_small_radius()) && is_equal(d, hc1.get_offset()));
}
 
TEST(get_point, point)
{
    Hc hc(100.001, 10.02, 3.99, 0, 0);
    double x = 92.4294, y = -2.98175;
    point p = hc.get_point_by_angle(12.5);
    
    ASSERT_NEAR(p.x, x, 0.0001);
    ASSERT_NEAR(p.y, y, 0.0001);
}
 

TEST(get_area, area)
{
    Hc hc(10.00, 1.02, 3.1, 0, 0);
    ASSERT_NEAR(17.3969, hc.get_sector_area(14.01), 0.001);
}
 
TEST(radius_of_curvature, rad)
{
    Hc hc(1000.92, 100.22, 43, 0, 0);
    ASSERT_NEAR(380.112, hc.get_radius_of_curvature(1.21), 0.001);
}

TEST(limited_radius, radrad)
{
    Hc hc(14.01, 1.54, 4, 0, 0);
    double x = 8.47;
    double y = 16.47;
    point p = hc.get_limitimg_circles_radius();
    ASSERT_NEAR(x, p.x, 0.001);
    ASSERT_NEAR(y, p.y, 0.001);
}
 
int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

