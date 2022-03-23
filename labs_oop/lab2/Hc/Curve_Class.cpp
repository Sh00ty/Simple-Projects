//
//  Curve_Class.cpp
//  lab2
//
//  Created by Павел on 16.09.2021.
//

#include "Curve_Class.hpp"


point::point() = default;
point::point(double x, double y): x(x), y(y) {};
point::point(const point& point): x(point.x), y(point.y) {};


/*
 R is the radius of a stationary circle.
 r is the radius of a rolling circle.
 d is the offset
 */

    
Hc::Hc() = default;
Hc::Hc(double R, double r, double d, double x, double y): R(R), r(r), d(d), x(x), y(y) {};
Hc::Hc(const Hc& hcp): Hc(hcp.R, hcp.r, hcp.d, hcp.x, hcp.y) {};
        
    
Hc& Hc::operator=(const Hc& tmp)
{
    R = tmp.R;
    r = tmp.r;
    d = tmp.d;
    x = tmp.x;
    y = tmp.y;
    return *this;
}
    
    
point Hc::get_point_by_angle(double angle) const
{
    point p;
    p.x = x + (R - r) * cos(angle) + d * cos((R - r) * angle / r);
    p.y = y + (R - r) * sin(angle) - d * sin((R - r) * angle / r);
    return p;
}

    
int Hc::what_type() const
{
    if (fabs(r - d) < std::numeric_limits<double>::epsilon())
    {
        std::cout << "This is a common hypocycloid" << std::endl;
        return 0;
    }
    else if (d < r)
    {
        std::cout << "This is a shortended hypocycloid" << std::endl;
        return -1;
    }
    std::cout << "This is a elongated hypocycloid" << std::endl;
    return 1;
}
        
    
double Hc::get_small_radius() const
{
    return r;
}
    
double Hc::get_big_radius() const
{
    return R;
}
    
double Hc::get_offset() const
{
    return d;
}
    
double Hc::get_radius_of_curvature(double angle) const
{
    return (R - r) * pow(pow(r, 2) + pow(d, 2) - 2 * r * d * cos(R * angle / r), 1.5) / abs(-pow(r, 3) + pow(d, 2) * (R - r) - d * r * (R - 2*r) * cos(R * angle) / r);
}
    
double Hc::get_sector_area(double angle) const
{
    return abs((R - r) * ((R - r - pow(d,2) / r) * angle - d * (R - 2 * r) / R * sin(-1* R * angle) / r) / 2);
}
    
    // return point where  x = small_lim_radius, y = big_lim_radius
point Hc::get_limitimg_circles_radius() const
{
    point res(abs(R - r - d), abs(R - r + d));
    return res;
}
    
bool Hc::is_correct()
{
    if (r <= 0 || R <= 0 || d <= 0 || R < r)
        return false;
    return true;
}
