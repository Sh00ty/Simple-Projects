//
//  main.cpp
//  cpp_test
//
//  Created by Павел on 04.09.2021.
//

#include <iostream>
#include "MatrixClass.hpp"
#include "funcs.hpp"

std::ostream& operator<<(std::ostream& cout, const Matrix& matrix)
{
    matrix.print();
    return cout;
}

std::ostream& operator<<(std::ostream& cout, const Matrix::point* point)
{
    return cout << "X:: " << point->x << " Y:: " << point->y << " Val:: " << point->value;
}


int main()
{
    Matrix mat;
    
    if(!get_mat(mat))
    {
        std::cout << "\nProgramm closed succsesfully\n";
        return 0;
    }
    
    mat.resize();
    std::cout << mat;
    
    int64_t* v = main_algo(mat);
    
    std::cout << "------------" << std::endl;
    if (v == nullptr)
    {
        for(size_t i = 0; i < mat.getrows(); ++i) std::cout << "0" << std::endl;
        return 0;
    }
    for (size_t i = 0; i < mat.getrows(); ++i)
        std::cout << v[i] << std::endl;
    
    delete [] v;
    return 0;
}
