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
#include <iterator>
#include <memory>
#include <algorithm>
#include <utility>


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
    
    
    SmartPtr<T>& operator=(const SmartPtr<T>& p){
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
        if (this == &p) return *this;
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
    
    const T& operator*() const noexcept{
        if (*size > 1)
            return ptr[0];
        return *ptr;
    }
    
    const T* operator->() const noexcept{
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
        try{
            if (!size) size = new size_t;
            refs = new RF;
        }catch(std::exception & ba){
            (new_size == 1) ? delete p : delete[] p;
            throw;
        }
        *size = new_size;
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

#endif /* MyLib_hpp */
