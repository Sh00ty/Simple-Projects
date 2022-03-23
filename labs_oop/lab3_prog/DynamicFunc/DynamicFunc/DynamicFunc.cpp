//
//  DynamicFunc.cpp
//  DynamicFunc
//
//  Created by Pavel Shlykov B20-515 on 01.10.2021.
//

#include "DynamicFunc.hpp"


namespace lab3D{

// POINT        POINT       POINT       POINT       POINT        POINT       POINT       POINT
// POINT        POINT       POINT       POINT       POINT        POINT       POINT       POINT
// POINT        POINT       POINT       POINT       POINT        POINT       POINT       POINT

point& point::operator=(const point& p){
    x = p.x;
    y = p.y;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const lab3D::point& p){
    return out << "(" << p.x << ", " << p.y << ")";
}


// DynamicFunc      DynamicFunc     DynamicFunc     DynamicFunc     DynamicFunc     DynamicFunc
// DynamicFunc      DynamicFunc     DynamicFunc     DynamicFunc     DynamicFunc     DynamicFunc
// DynamicFunc      DynamicFunc     DynamicFunc     DynamicFunc     DynamicFunc     DynamicFunc

DynamicFunction::DynamicFunction(const point& p){
    points.create_ptr(new point(p));
    ++size;
}


DynamicFunction::DynamicFunction(size_t count, point* arr): size(count){
    if (count == 0) throw std::invalid_argument("size of array can't be below zero");
    
    if(!arr) throw std::invalid_argument("can't copy nullptr array");
    
    (count == 1) ? points.create_ptr(new point) : points.create_ptr(new point[count], count);
    size_t j = 0;
    for (size_t i = 0; i < count; ++i){
        size_t res = MLT::insert_in_sort(points.get_ptr(), j, arr[i], point::point_comparator);
        if (res != size_t(-1)) ++j;
    }
    if (j != count){
        points.realloc(j);
        size = j;
    }
}

//------------------------------------------------

DynamicFunction::DynamicFunction(const DynamicFunction& f): size(f.size){
    if (f.size != 0){
        (f.size == 1) ? points.create_ptr(new point) : points.create_ptr(new point[f.size], f.size);
        std::copy(f.points.get_ptr(), f.points.get_ptr() + f.size, points.get_ptr());
    }
}


DynamicFunction& DynamicFunction::operator=(const DynamicFunction& df){
    if (this == &df) return *this;
    DynamicFunction tmp = df;
    std::swap(points, tmp.points);
    std::swap(size, tmp.size);
    return *this;
}


DynamicFunction::DynamicFunction(DynamicFunction&& df): size(df.size){
    std::swap(points, df.points);
    df.size = 0;
}


DynamicFunction& DynamicFunction::operator=(DynamicFunction&& df){
    DynamicFunction tmp = std::move(df);
    std::swap(points, tmp.points);
    std::swap(size, tmp.size);
    return *this;
}

//------------------------------------------------


DynamicFunction::iterator DynamicFunction::add(const point& p){
    if (size == 0){
        points.create_ptr(new point(p));
        ++size;
        return iterator(points.get_ptr());
    }
    points.realloc(size + 1);
    size_t res = MLT::insert_in_sort(points.get_ptr(), size++, p, point::point_comparator);
    if (res == size_t(-1)){
        --size;
        points.realloc(size);
        return end();
    }
    return iterator(points.get_ptr() + res);
}

//------------------------------------------------

double DynamicFunction::max() const{
    if (size == 0)
        throw std::logic_error("Function does not have a mimimun value");
    
    double max = std::numeric_limits<double>::max();
    max *= -1;
    for(size_t i = 0; i < size; ++i)
        if(max < points[i].y) max = points[i].y;
    return max;
}


double DynamicFunction::min() const{
    if (size == 0)
        throw std::logic_error("Function does not have a maximum value");
    
    double min = std::numeric_limits<double>::max();
    for(size_t i = 0; i < size; ++i)
        if(min > points[i].y) min = points[i].y;
    return min;
}

//------------------------------------------------

// return 1 if func is increasing -1 if decreasing and 0 if not monotonous and 2 if const
char DynamicFunction::type() const{
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
    
//------------------------------------------------

// build a straight line between the closest points and there it defines the value
double DynamicFunction::value(double x) const {
    if(size == 0)
        throw std::logic_error("Function has not been created yet");
    
    if (points[0].x > x)
        throw std::out_of_range("Function is not defined in this scope");
    point p(x, 0);
    
    size_t ind = MLT::bin_search(points.get_ptr(), size, p, point::point_comparator);
    
    if (MLT::is_equal(points[ind].x, x))
        return points[ind].y;
    else if (ind == size - 1)
        throw std::out_of_range("the function is not defined in this scope");
    else
        return (points[ind].y + (points[ind + 1].y - points[ind].y) / (points[ind + 1].x - points[ind].x) * (x - points[ind].x));
}


//------------------------------------------------

// print() and input() are usefull for abstact classes(but in general it can do without them)

std::ostream& DynamicFunction::print(std::ostream& out) const noexcept{
    for (size_t i = 0; i < size; ++i){
        out << "point " << i << ":: " << points[i] << std::endl;
    }
    return out;
}


std::istream& DynamicFunction::input(std::istream& in){
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


std::ostream& operator<<(std::ostream& out, const DynamicFunction& f){
    return f.print(out);
}

std::istream& operator>>(std::istream& in, DynamicFunction& f){
    return f.input(in);
}


}

