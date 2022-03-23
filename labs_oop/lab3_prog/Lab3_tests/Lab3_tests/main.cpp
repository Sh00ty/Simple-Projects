//
//  main.cpp
//  Lab3_tests
//
//  Created by Павел on 03.10.2021.
//

#include <iostream>
#include <gtest/gtest.h>
#include <MyLib.hpp>
#include "StaticFunc.hpp"
#include "DynamicFunc.hpp"


// !!!!!!   i made tests for static and dynamic cases together    !!!!!!
//              all methods except adding work the same


class TestFunc : public ::testing::Test
{
protected:
    void SetUp(){
    // increasing function
        sf = new lab3S::StaticFunction;
        sf->add(lab3S::point(-1, -2.02151));
        sf->add(lab3S::point(0.0001, 0.2));
        sf->add(lab3S::point(1, 2));
        sf->add(lab3S::point(4, 3));
        sf->add(lab3S::point(4, 2));
        sf->add(lab3S::point(7, 8));
        
    // decreasing function
        sd = new lab3D::DynamicFunction;
        sd->add(lab3D::point(-10, 101.01));
        sd->add(lab3D::point(10, -0.2));
        sd->add(lab3D::point(0, 0));
        sd->add(lab3D::point(-1, 1));
        sd->add(lab3D::point(-2, 2));
        sd->add(lab3D::point(7, -0.1));
    
    // not monotonous
        sd1 = new lab3D::DynamicFunction;
        sd1->add(lab3D::point(-113, -2));
        sd1->add(lab3D::point(0, 0.2));
        sd1->add(lab3D::point(1, 22));
        sd1->add(lab3D::point(4, 30));
        sd1->add(lab3D::point(4, 8));
        sd1->add(lab3D::point(7, 8));
    
    // const function
        sd2 = new lab3D::DynamicFunction;
        sd2->add(lab3D::point(1, 1.0001));
        sd2->add(lab3D::point(0.0001, 1.0001));
        sd2->add(lab3D::point(5, 1.0001));
        sd2->add(lab3D::point(99.99, 1.0001));
        sd2->add(lab3D::point(100, 1.0001));
        sd2->add(lab3D::point(-1000.01, 1.0001));
    
    }
    
    void TearDown(){
        delete sf;
        delete sd;
        delete sd1;
        delete sd2;
    }
    
    lab3S::StaticFunction* sf;
    lab3D::DynamicFunction* sd;
    lab3D::DynamicFunction* sd1;
    lab3D::DynamicFunction* sd2;
    
};


TEST_F(TestFunc, add_and_value_exactly_at_the_point){
    
    ASSERT_NEAR(0.2, sf->value(0.0001), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(3, sf->value(4), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(2, sf->value(1), std::numeric_limits<double>::epsilon());
    
    
    ASSERT_NEAR(-0.2, sd->value(10), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(0, sd->value(0), std::numeric_limits<double>::epsilon());
    
    ASSERT_NEAR(-2, sd1->value(-113), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(0.2, sd1->value(0), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(22, sd1->value(1), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(30, sd1->value(4), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(8, sd1->value(7), std::numeric_limits<double>::epsilon());
    
    ASSERT_NEAR(1.0001, sd2->value(5), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(1.0001, sd2->value(99.99), std::numeric_limits<double>::epsilon());
}

TEST_F(TestFunc, type){
    ASSERT_EQ(1, sf->type());
    ASSERT_EQ(-1, sd->type());
    ASSERT_EQ(0, sd1->type());
    ASSERT_EQ(2, sd2->type());
}

TEST_F(TestFunc, value){

    ASSERT_NEAR(8.0/3.0, sf->value(3), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(14.0/3.0, sf->value(5), std::numeric_limits<double>::epsilon());
    
    ASSERT_NEAR(46.0/3.0, sd1->value(6), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(30, sd1->value(4), std::numeric_limits<double>::epsilon());
    
    ASSERT_NEAR(1.0001, sd2->value(5.01413), std::numeric_limits<double>::epsilon());
    
    ASSERT_THROW(sd1->value(-114), std::out_of_range);
    ASSERT_THROW(sd2->value(100.001), std::out_of_range);
}

TEST_F(TestFunc, min){
    ASSERT_NEAR(-2.02151, sf->min(), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(-0.2, sd->min(), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(-2, sd1->min(), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(1.0001, sd2->min(), std::numeric_limits<double>::epsilon());
}

TEST_F(TestFunc, max){
    ASSERT_NEAR(8, sf->max(), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(101.01, sd->max(), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(30, sd1->max(), std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(1.0001, sd2->max(), std::numeric_limits<double>::epsilon());
}



 
int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


