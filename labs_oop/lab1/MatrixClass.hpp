//
//  MatrixClass.hpp
//  cpp_test
//
//  Created by Павел on 07.09.2021.
//



#ifndef MatrixClass_hpp
#define MatrixClass_hpp

#include <iostream>
#include <cstring>

class Matrix
{
public:
    struct point
    {
        int x = -1;
        int y = -1;
        int value = - 1;
    };
private:
    point* mat = nullptr;
    size_t size = 0;
    size_t colums = 0;
    size_t rows = 0;
    size_t count = 0;
public:
    
    Matrix();
    
    Matrix(size_t size, size_t colums, size_t rows);
    
    Matrix(const Matrix& matrix);
    
    ~Matrix();
    
    void add(int x, int y, int val);
    void del();
    void print() const;
    size_t getsize() const;
    size_t getrows() const;
    size_t getcolums() const;
    point* get(size_t ind) const;
    Matrix& operator=(Matrix matrix);
    //Matrix& resize();
    void resize();
};


#endif
/* MatrixClass_hpp */


