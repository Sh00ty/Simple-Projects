//
//  FuncClass.hpp
//  lab3
//
//  Created by Pavel Shlykov B20-515 on 25.09.2021.
//

#ifndef FuncClass_hpp
#define FuncClass_hpp

#include <iostream>
#include "MyLib.hpp"


// !!!!!    MLT = MyLib.hpp     !!!!!

namespace lab3S{


struct point{
    double x = 0;
    double y = 0;
        
    point(double x = 0, double y = 0): x(x), y(y) {}
    
    // i made comparator for sort and bin_search
    static bool point_comparator(const point& p1, const point& p2){
        return !(p1.x > p2.x);
    }
    
    point& operator=(const point& p);
};


// class defines a discrete function defined only at certain points
class StaticFunction{
private:
    static const size_t func_capacity = 5;
    point points[func_capacity];
    size_t size = 0;
public:
    StaticFunction() = default;
    // accepts any array of points.
    explicit StaticFunction(size_t count, point* arr);
    StaticFunction(const point& p);

    // adds one point and if it can't return size_t(-1)
    size_t add(const point& p);
    
    double max() const;
    double min() const;
    
    // return 1 if func is increasing -1 if decreasing and 0 if not monotonous and 2 if const
    char type() const;
    
    //returns value in any point except (x < min(x) and x > max(x))
    //(build a straight line between the closest points and there it defines the value)
    double value(double x) const;

    
    std::ostream& print(std::ostream& out) const noexcept;
    std::istream& input(std::istream& in);
};


std::ostream& operator<<(std::ostream& out, const StaticFunction& f);
std::istream& operator>>(std::istream& in, StaticFunction& f);

}


#endif /* FuncClass_hpp */
