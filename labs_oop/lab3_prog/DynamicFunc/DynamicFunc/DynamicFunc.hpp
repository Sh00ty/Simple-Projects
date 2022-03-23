//
//  DynamicFunc.hpp
//  DynamicFunc
//
//  Created by Pavel Shlykov B20-515 on 01.10.2021.
//

#ifndef DynamicFunc_hpp
#define DynamicFunc_hpp

#include <iostream>
#include <MyLib.hpp>

// !!!!!    MLT = MyLib.hpp     !!!!!



namespace lab3D{


struct point{
    double x = 0;
    double y = 0;
        
    point(double x = 0, double y = 0): x(x), y(y) {}
    point(const point& p): x(p.x), y(p.y) {}
    
    // i made comparator for sort and bin_search
    static bool point_comparator(const point& p1, const point& p2){
        return !(p1.x > p2.x);
    }
    
    point& operator=(const point& p);
};


// class defines a discrete function defined only at certain points
class DynamicFunction{
private:
    
    template<bool is_const>
    class Any_iterator{
        MLT::conditional_t<is_const, const point, point>* it;
        
    public:
        Any_iterator(MLT::conditional_t<is_const, const point, point>* p): it(p) {}
        
        MLT::conditional_t<is_const, const Any_iterator, Any_iterator>& operator+=(int64_t n){
            it += n;
            return *this;
        }
        
        MLT::conditional_t<is_const, const Any_iterator, Any_iterator>& operator-=(int64_t n){
            it -= n;
            return *this;
        }
        
        MLT::conditional_t<is_const, const Any_iterator, Any_iterator>& operator++(){
            ++it;
            return *this;
        }
        MLT::conditional_t<is_const, const Any_iterator, Any_iterator>& operator--(){
            --it;
            return *this;
        }
        
        MLT::conditional_t<is_const, const point, point>& operator*(){
            return *it;
        }
        
        bool operator==(const Any_iterator& iter){
            return it == iter.it;
        }
        
        bool operator!=(const Any_iterator& iter){
            return !(*this == iter);
        }
    };
    
    using const_iterator = Any_iterator<true>;
    using iterator = Any_iterator<false>;
    
    // SmartPtr deletes memory itself
    MLT::SmartPtr<point> points;
    size_t size = 0;
public:
    DynamicFunction() = default;
    
    // accepts any array of points.
    explicit DynamicFunction(size_t count, point* arr);
    DynamicFunction(const point& p);
    
    DynamicFunction(const DynamicFunction& DynamicFunction);
    DynamicFunction& operator=(const DynamicFunction& df);
    
    DynamicFunction(DynamicFunction&& df);
    DynamicFunction& operator=(DynamicFunction&& df);
    
    ~DynamicFunction() = default;
    
    // adds one point and if it can't return iterator end()
    iterator add(const point& p);
    
    double max() const;
    double min() const;
    
    // return 1 if func is increasing -1 if decreasing and 0 if not monotonous and 2 if const
    char type() const;
    
    // returns value in any point except (x < min(x) and x > max(x))
    // (build a straight line between the closest points and there it defines the value)
    double value(double x) const;
    
    iterator begin(){
        iterator it(points.get_ptr());
        return it;
    }
    
    iterator end(){
        iterator it(points.get_ptr() + size);
        return it;
    }
    
    const_iterator begin() const{
        const_iterator it(points.get_ptr());
        return it;
    }
    
    const_iterator end() const{
        const_iterator it(points.get_ptr() + size);
        return it;
    }
    
    std::ostream& print(std::ostream& out) const noexcept;
    std::istream& input(std::istream& in);
};


std::ostream& operator<<(std::ostream& out, const DynamicFunction& f);
std::istream& operator>>(std::istream& in, DynamicFunction& f);

std::ostream& operator<<(std::ostream& out, const lab3D::point& p);
}

#endif /* DynamicFunc_hpp */
