//
//  Curve_Class.hpp
//  lab2
//
//  Created by Павел on 16.09.2021.
//

#ifndef Curve_Class_hpp
#define Curve_Class_hpp

#include <iostream>
#include <math.h>

struct point
{
    double x = 0;
    double y = 0;
    
    point();
    point(double x, double y);
    point(const point& point);
};


/*
 R is the radius of a stationary circle.
 r is the radius of a rolling circle.
 d is the offset
 */
class Hc
{
private:
    double R = 0;
    double r = 0;
    double d = 0;
    double x = 0;
    double y = 0;
public:
    
    Hc();
    Hc(double R, double r, double d, double x, double y);
    Hc(const Hc& hcp);
    
    Hc& operator=(const Hc& tmp);
    
    
    point get_point_by_angle(double angle) const;

    
    int what_type() const;
        
    
    double get_small_radius() const;
    
    double get_big_radius() const;
    
    double get_offset() const;
    
    double get_radius_of_curvature(double angle) const;
    
    double get_sector_area(double angle) const;
    
    // return point where  x = small_lim_radius, y = big_lim_radius
    point get_limitimg_circles_radius() const;
    
    bool is_correct();
};

#endif /* Curve_Class_hpp */
