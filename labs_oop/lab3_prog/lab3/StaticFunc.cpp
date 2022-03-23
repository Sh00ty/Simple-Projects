//
//  FuncClass.cpp
//  lab3
//
//  Created by Pavel Shlykov B20-515  on 25.09.2021.
//

#include "StaticFunc.hpp"

namespace lab3S{
    
// POINT        POINT       POINT       POINT       POINT        POINT       POINT       POINT
// POINT        POINT       POINT       POINT       POINT        POINT       POINT       POINT
// POINT        POINT       POINT       POINT       POINT        POINT       POINT       POINT


point& point::operator=(const point& p){
    x = p.x;
    y = p.y;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const lab3S::point& p){
    return out << "(" << p.x << ", " << p.y << ")";
}

// StaticFunc      StaticFunc     StaticFunc     StaticFunc     StaticFunc     StaticFunc
// StaticFunc      StaticFunc     StaticFunc     StaticFunc     StaticFunc     StaticFunc
// StaticFunc      StaticFunc     StaticFunc     StaticFunc     StaticFunc     StaticFunc

StaticFunction::StaticFunction(const point& p){
    points[0] = p;
    ++size;
}


StaticFunction::StaticFunction(size_t count, point* arr): size(count){
    if (count == 0) throw std::invalid_argument("size of array can't be below zero");
    
    if (count > func_capacity){
        count = func_capacity;
    }

    size_t j = 0;
    for (size_t i = 0; i < count; ++i){
        size_t res = MLT::insert_in_sort(points, j, arr[i], point::point_comparator);
        if (res != size_t(-1)) ++j;
    }
    if (count != j) size = j;
}


size_t StaticFunction::add(const point& p){
    if (size == func_capacity)
        throw std::out_of_range("the number of nodes is already equal to the capacity");
    size_t res = MLT::insert_in_sort(points, size++, p, point::point_comparator);
    if (res == size_t(-1)){
        --size;
    }
    return res;
}


double StaticFunction::max() const{
    if (size == 0)
        throw std::logic_error("Function does not have a mimimun value");
    
    double max = std::numeric_limits<double>::max();
    max *= -1;
    for(size_t i = 0; i < size; ++i)
        if(max < points[i].y) max = points[i].y;
    return max;
}


double StaticFunction::min() const{
    if (size == 0)
        throw std::logic_error("Function does not have a maximum value");
    
    double min = std::numeric_limits<double>::max();
    for(size_t i = 0; i < size; ++i)
        if(min > points[i].y) min = points[i].y;
    return min;
}


// return 1 if func is increasing -1 if decreasing and 0 if not monotonous and 2 if const
char StaticFunction::type() const{
    if (size == 0)
        throw std::logic_error("Function does not have any value");

    char status = 3;
    for(size_t i = 1; i < size; ++i){
        if (points[i].y > points[i-1].y){
            if (status == 3) status = 1;
            else if (status == -1 || status == 2) return 0;
        }else if (points[i].y < points[i-1].y){
            if (status == 3) status = -1;
            else if (status == 1 || status == 2) return 0;
        }else{
            if (status == 3) status = 2;
            if (status == -1 || status == 1) return 0;
        }
    }
    return status;
}
    
    
double StaticFunction::value(double x) const {
    if(size == 0)
        throw std::logic_error("Function has not been created yet");
    
    if (points[0].x > x)
        throw std::out_of_range("Function is not defined in this scope");
    point p(x, 0);
    
    size_t ind = MLT::bin_search(points, size, p, point::point_comparator);
    
    if (MLT::is_equal(points[ind].x, x))
        return points[ind].y;
    else if (ind == size - 1)
        throw std::out_of_range("the function is not defined in this scope");
    else
        return (points[ind].y + (points[ind + 1].y - points[ind].y) / (points[ind + 1].x - points[ind].x) * (x - points[ind].x));
}

std::ostream& StaticFunction::print(std::ostream& out) const noexcept{
    for (size_t i = 0; i < size; ++i){
        out << "point " << i << ":: " << points[i] << std::endl;
    }
    return out;
}


std::ostream& operator<<(std::ostream& out, const StaticFunction& f){
    return f.print(out);
}


std::istream& StaticFunction::input(std::istream& in){
    size_t num;
    if (!(in >> num)){
        in.setstate(std::ios::failbit);
        return in;
    }
    
    for (size_t i = 0; i < num; ++i){
        point p;
        if (!(in >> p.x)){
            in.setstate(std::ios::failbit);
            return in;
        }
        if (!(in >> p.y)){
            in.setstate(std::ios::failbit);
            return in;
        }
        this->add(p);
    }
    return in;
}


std::istream& operator>>(std::istream& in, StaticFunction& f){
    return f.input(in);
}


}

