//
//  MyLib.hpp
//  MySpace
//
//  Created by Pavel Shlykov B20-515 on 21.09.2021.
//

#ifndef MyLib_hpp
#define MyLib_hpp

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstring>
#include <math.h>


namespace MLT{

// Smart pointer        Smart pointer       Smart pointer       Smart pointer       Smart pointer       Smart pointer
// Smart pointer        Smart pointer       Smart pointer       Smart pointer       Smart pointer       Smart pointer
// Smart pointer        Smart pointer       Smart pointer       Smart pointer       Smart pointer       Smart pointer

template<typename T>
class SmartPtr{

    struct RF{
        size_t refs = 1;
        
        RF() = default;
        
        size_t add_ref(){
            return ++refs;
        }
        size_t del_ref(){
            return --refs;
        }
    };
    
    T* ptr = nullptr;
    RF* refs = nullptr;
    size_t* size = nullptr;
public:
    SmartPtr() = default;
    
    explicit SmartPtr(T* p, size_t new_size = 1): ptr(p), refs(new RF) ,size(new size_t(new_size)){}
    
    //------------------------------------------------
    
    SmartPtr(const SmartPtr<T>& pointer): ptr(pointer.ptr), refs(pointer.refs), size(pointer.size){
        if (!pointer.refs) return;
        refs->add_ref();
    }
    
    
    SmartPtr<T>& operator=(SmartPtr<T>& p){
        if (&p == this) return *this;
        SmartPtr<T> tmp = p;
        std::swap(tmp.ptr, ptr);
        std::swap(tmp.refs, refs);
        std::swap(tmp.size, size);
        return *this;
    }
    
    //------------------------------------------------
    
    SmartPtr(SmartPtr<T>&& p): ptr(p.ptr), refs(p.refs), size(p.size){
        p.ptr = nullptr;
        p.refs = nullptr;
        p.size = nullptr;
    }
    
    
    SmartPtr& operator=(SmartPtr<T>&& p){
        SmartPtr<T> tmp = std::move(p);
        std::swap(tmp.ptr, ptr);
        std::swap(tmp.refs, refs);
        std::swap(tmp.size, size);
        return *this;
    }
    //------------------------------------------------
    
    ~SmartPtr(){
        if (!refs) return;
        if (refs->refs > 1) refs->del_ref();
        else if (*size == 1){
            delete ptr;
            delete refs;
            delete size;
        }else if (*size > 1) {
            delete[] ptr;
            delete refs;
            delete size;
        }
    }
    
    //------------------------------------------------
    
    size_t get_size() const noexcept{
        return ((size) ? *size : 0);
    }

    
    //------------------------------------------------
    
    T& operator*() noexcept{
        if (*size > 1)
            return ptr[0];
        return *ptr;
    }
    
    T* operator->() noexcept{
        return ptr;
    }
    
    //------------------------------------------------
    
    T& operator[](size_t index) noexcept{
        return ptr[index];
    }
    
    const T& operator[](size_t index) const noexcept{
        return ptr[index];
    }
    
    T& af(size_t index){
        if (index >= *size)
            throw std::out_of_range(std::string("SmartPtr:: index out of range (") + std::to_string(index) + ") ");
        return ptr[index];
    }
    
    const T& af(size_t index) const{
        if (index >= *size)
            throw std::out_of_range(std::string("SmartPtr:: index out of range (") + std::to_string(index) + ") ");
        return ptr[index];
    }
    
    //------------------------------------------------
    
    T* get_ptr() noexcept{
        return ptr;
    }
    
    const T* get_ptr() const noexcept{
        return ptr;
    }
        
    //------------------------------------------------
    
    void realloc(size_t new_size){
        T* newarr = nullptr;
        if (new_size == 0) this->del();
        
        (new_size == 1) ? newarr = new T : newarr = new T[new_size];
        
        //(*size >= new_size) ? memcpy(newarr, ptr, new_size * sizeof(T)) : memcpy(newarr, ptr, *size * sizeof(T));
        try{
            size_t min = (new_size >= *size ? *size : new_size);
            for (size_t i = 0; i < min; ++i){
                new (newarr + i) T(std::move(ptr[i]));
            }
        }catch(...){
            (new_size == 1) ? delete newarr : delete [] newarr;
        }
        
        for (size_t i = 0; i < *size; ++i)
            (ptr + i)->~T();
            
        (*size == 1) ? delete ptr : delete[] ptr;
        
        ptr = newarr;
        *size = new_size;
    }
    
    
    //------------------------------------------------
    
    void create_ptr(T* p, size_t new_size = 1){
        if (ptr) return;
        ptr = p;
        if (!size) size = new size_t;
        *size = new_size;
        refs = new RF;
    }
    
    void del() noexcept{
        if (refs->refs > 1)
            refs->del_ref();
        else if (*size == 1){
            delete refs;
            delete ptr;
            delete size;
        }else if (*size > 0){
            delete refs;
            delete[] ptr;
            delete size;
        }
        refs = nullptr;
        size = nullptr;
        ptr = nullptr;
    }
    
};

// TYPE TRAITS    TYPE TRAITS     TYPE TRAITS     TYPE TRAITS     TYPE TRAITS     TYPE TRAITS     TYPE TRAITS
// TYPE TRAITS    TYPE TRAITS     TYPE TRAITS     TYPE TRAITS     TYPE TRAITS     TYPE TRAITS     TYPE TRAITS
// TYPE TRAITS    TYPE TRAITS     TYPE TRAITS     TYPE TRAITS     TYPE TRAITS     TYPE TRAITS     TYPE TRAITS

template<typename T, typename U>
struct is_same{
    static const bool result = false;
};

template<typename T>
struct is_same<T, T>{
    static const bool result = true;
};

template<typename T, typename U>
const bool is_same_v = is_same<T, U>::result;

//------------------------------------------------


template<typename T>
bool is_unsigned(){
    return T(0) < T(-1);
}

//------------------------------------------------

template<bool T, typename U, typename V>
struct conditional{
    using type = V;
};


template<typename U, typename V>
struct conditional<true, U, V>{
    using type = U;
};

template<bool T, typename U, typename V>
using conditional_t = typename conditional<T, U, V>::type;

//------------------------------------------------

template<typename T>
struct remove_const{
    using type = T;
};

template<typename T>
struct remove_const<const T>{
    using type = T;
};

template<typename T>
using remove_const_t = typename remove_const<T>::type;


// ALOCATORS        ALOCATORS       ALOCATORS       ALOCATORS       ALOCATORS       ALOCATORS       ALOCATORS
// ALOCATORS        ALOCATORS       ALOCATORS       ALOCATORS       ALOCATORS       ALOCATORS       ALOCATORS
// ALOCATORS        ALOCATORS       ALOCATORS       ALOCATORS       ALOCATORS       ALOCATORS       ALOCATORS
/*
template<typename T>
class PoolAllocator{
    T* memory = nullptr;
    size_t size;
    size_t ind;
public:
    T* allocate();
    void deallocate();
    void constract();
    void destruct();
};
*/

// MY EXCEPTIONS        MY EXCEPTIONS       MY EXCEPTIONS       MY EXCEPTIONS       MY EXCEPTIONS       MY EXCEPTIONS
// MY EXCEPTIONS        MY EXCEPTIONS       MY EXCEPTIONS       MY EXCEPTIONS       MY EXCEPTIONS       MY EXCEPTIONS
// MY EXCEPTIONS        MY EXCEPTIONS       MY EXCEPTIONS       MY EXCEPTIONS       MY EXCEPTIONS       MY EXCEPTIONS

class Excep: public std::exception{
private:
    std::string message;
public:
    Excep(const char* str = "something went wrong"): message(str) {}
    
    const char* what() const noexcept override{
        return message.c_str();
    }
    
};


class Eof_error: public MLT::Excep{
private:
    std::string message;
public:
    Eof_error(const char* str = "something went wrong"): message(str) {}
    
    const char* what() const noexcept override{
        return message.c_str();
    }
    
};



// FUNCS        FUNCS       FUNCS       FUNCS       FUNCS       FUNCS       FUNCS       FUNCS       FUNCS       FUNCS
// FUNCS        FUNCS       FUNCS       FUNCS       FUNCS       FUNCS       FUNCS       FUNCS       FUNCS       FUNCS
// FUNCS        FUNCS       FUNCS       FUNCS       FUNCS       FUNCS       FUNCS       FUNCS       FUNCS       FUNCS


template<typename T>
T get_num(std::istream& in = std::cin, std::ostream& os = std::cout){
    T num;
    if (is_unsigned<T>()){
        int64_t tmp;
        while(!(in >> tmp) || tmp < 0){
            if (in.eof())
                throw MLT::Eof_error("programm will close!!!");
            os << std::endl << "Enter positive number\n::";
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        num = static_cast<T>(tmp);
    }else{
        while(!(in >> num)){
            if (in.eof())
                throw MLT::Eof_error("programm will close!!!");
            os << std::endl << "Enter failed, try again\n::";
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return num;
}

template<typename T>
bool bigger_cmp(const T& a, const T& b) noexcept{
    return a >= b;
}


template<typename T>
bool smaller_cmp(const T& a, const T& b) noexcept{
    return a <= b;
}


// reallocate memory with dump memory(if it wasn't dump, so it would be)
template<typename T>
T* realloc_damp(T* arr, size_t old_size, size_t new_size, bool is_dump = true){
    
    T* newarr = reinterpret_cast<T*>(new int8_t [new_size * sizeof(T)]);
    try{
        size_t min = (new_size >= old_size ? old_size : new_size);
        for (size_t i = 0; i < min; ++i){
            new(newarr + i) T(std::move(arr[i]));
        }
    }catch(...){
        delete[] reinterpret_cast<int8_t*>(newarr);
        return nullptr;
    }
    
    for (size_t i = 0; i < old_size; ++i){
        (arr + i)->~T();
    }
    if (old_size > 0)
        (is_dump ? delete[] reinterpret_cast<int8_t*>(arr) : delete [] arr);
    
    return newarr;
}


template<typename T>
int64_t bin_search(const T* arr, size_t size, const T& val, bool (*cmp)(const T&, const T&)) noexcept{
    int64_t a = -1;
    int64_t b = size;
    while (a + 1 < b){
        size_t mid = (a + b) / 2;
        if (cmp(arr[mid], val) && cmp(val, arr[mid])) return mid;
        else if (cmp(arr[mid], val)) a = mid;
        else b = mid;
    }
    return a;
}


template<typename T>
int64_t insert_in_sort(T* arr, size_t size, const T& val, bool (*cmp)(const T&, const T&)) noexcept{
    if (size == 0){
        arr[0] = val;
        return 0;
    }
    int64_t ind = bin_search(arr, size, val, cmp);
    if (ind != -1)
        if (cmp(arr[ind], val) && cmp(val, arr[ind])) return -1;
    memmove(arr + ind + 2, arr + ind + 1, (size - ind - 1) * sizeof(T));
    arr[ind + 1] = val;
    return ind + 1;
}

bool is_equal(double r, double d) noexcept;


// Classes      Classes     Classes     Classes     Classes     Classes     Classes     Classes     Classes     Classes
// Classes      Classes     Classes     Classes     Classes     Classes     Classes     Classes     Classes     Classes
// Classes      Classes     Classes     Classes     Classes     Classes     Classes     Classes     Classes     Classes

template<typename T>
class SegmentTree{
    T* tree;
    size_t size;
    T (*function)(T a, T b);
    T neutral_;
    
    // O(n)
    void tree_constarct(int64_t v, int64_t tl, int64_t tr, T* arr)
    {
        if (tl == tr) tree[v] = arr[tl];
        else{
            int64_t tm = (tl + tr) / 2;
            tree_constarct(2*v + 1, tl, tm, arr);
            tree_constarct(2*v + 2, tm + 1, tr, arr);
            tree[v] = function(tree[2*v + 1], tree[2*v + 2]);
        }
    }

    // O(log n)
    T func(int64_t v, int64_t tl, int64_t tr, int64_t l, int64_t r) const{
        if (l > r) return neutral_;
        if (l == tl && r == tr) return tree[v];
        int64_t tm = (tl + tr) / 2;
        return function(func(2*v + 1, tl, tm, l, std::min(r, tm)) , func(2*v + 2, tm + 1, tr ,std::max(l, tm + 1), r));
    }
                 
    // O(log n)
    // MAKE NON RECURSIVE!!!
    void tree_update(int64_t v, int64_t tl, int64_t tr, int64_t pos, T num){

        if (tl == tr){
            tree[v] = num;
            return;
        }
        int64_t tm = (tl + tr) / 2;
        if (tm >= pos) tree_update(2*v + 1, tl, tm, pos, num);
        else tree_update(2*v + 2, tm + 1, tr, pos, num);
        tree[v] = function(tree[2*v + 1], tree[2*v + 2]);
    }

public:
    
    SegmentTree(T* arr, size_t arr_size, T (*func)(T a, T b), T neutral){
        
        tree = reinterpret_cast<T*>(new int8_t[arr_size * 4 * sizeof(T)]);
        
        for (size_t i = 0; i < arr_size * 4; ++i){
            new (tree + i) T(neutral);
        }
        size = arr_size;
        function = func;
        neutral_ = neutral;
        tree_constarct(0, 0, arr_size - 1, arr);
    }
    
    SegmentTree(const SegmentTree& tr): size(tr.size), function(tr.function), neutral_(neutral_){
        if (size != 0)
            std::copy(tr.tree, tr.tree + tr.size * 4, tree);
    }
    
    
    SegmentTree& operator=(const SegmentTree& tr){
        if (this == &tr) return *this;
        SegmentTree tmp = tr;
        std::swap(tree, tmp.tree);
        std::swap(size, tmp.size);
        std::swap(function, tmp.function);
        std::swap(neutral_, tmp.neutral_);
        return *this;
    }
    
    SegmentTree(SegmentTree&& tr){
        tree = tr.tree;
        size = tr.size;
        function = tr.function;
        neutral_ = tr.neutral_;
        tr.tree = nullptr;
        tr.size = 0;
    }
    
    SegmentTree& operator=(SegmentTree&& tr){
        SegmentTree tmp = std::move(tr);
        std::swap(tree, tmp.tree);
        std::swap(size, tmp.size);
        std::swap(function, tmp.function);
        std::swap(neutral_, tmp.neutral_);
        return *this;
    }
    
    ~SegmentTree(){
        for (size_t i = 0; i < 4 * size; ++i){
            (tree + i)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(tree);
    }
    

    T operator()(size_t l, size_t r) const{
        if (size == 0 || tree == nullptr) return neutral_;
        if (l > r) std::swap(l, r);
        if (r >= size) r = size - 1;
        return func(0, 0, size - 1, l, r);
    }
    
    void update(int64_t pos, T obj){
        if (size == 0 || tree == nullptr || pos + 1 > size) return;
        tree_update(0, 0, size - 1, pos, obj);
    }
        

    void change(T* array, size_t sz, T (*func)(T a, T b), T neutral){
        if (sz != size){
            T* newarr = reinterpret_cast<T*>(new int8_t [4 * sz * sizeof(T)]);
                
            if (tree != nullptr) {
                for (size_t i = 0; i < size; ++i){
                    (tree + i)->~T();
                }
                delete[] reinterpret_cast<int8_t*>(tree);
            }
            tree = newarr;
        }
        
        for (size_t i = 0; i < sz * 4; ++i){
            new (tree + i) T(neutral);
        }
        
        function = func;
        neutral_ = neutral;
        size = sz;
        tree_constarct(0, 0, size - 1, array);
    }
    
    bool is_correct(){
        if (tree == nullptr || size == 0) return false;
        return true;
    }
};


};

#endif /* MyLib_hpp */
