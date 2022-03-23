//
//  funcs.cpp
//  cpp_test
//
//  Created by Павел on 09.09.2021.
//

#include "funcs.hpp"

bool get_mat(Matrix& mat)
{
    size_t m = 0;
    size_t n = 0;
    std::cout << "Enter m and n::" << std::endl;
    if (!MySpace::get_num(m) || !MySpace::get_num(n)) return false;
    
    if (n > 1e6 || m > 1e6 || m * n == 0)
    {
        std::cout << "\nYou entered too large size for matrix or enteres zero";
        return false;
    }
    Matrix matrix(n* m, n, m);
    int y = 0;
    int j = 0;
    for (size_t i = 0; i < n * m; ++i)
    {
        if (j == n)
        {
            j = 0;
            ++y;
        }
        int val = 0;
        std::cout << "Enter val::" << std::endl;
        if(!MySpace::get_num(val)) return false;
        if (val != 0)
            matrix.add(j, y, val);
        ++j;
    }
    mat = matrix;
    return true;
}


int64_t* main_algo(const Matrix& matrix)
{
    if (matrix.getsize() < 1) return nullptr;
    const Matrix::point* point;
    double arr[matrix.getrows()];
    int64_t* res  = new int64_t[matrix.getrows()];
    for (int i = 0; i < matrix.getrows(); ++i)
    {
        res[i] = 0;
        arr[i] = 0;
    }
    size_t count = 0;
    int y = 0;
    for (size_t i = 0; i < matrix.getsize(); ++i)
    {
        point = matrix.get(i);
        if (y != point->y)
        {
            // idk averenge with or without zeros value
            //arr[y] /= count;
            arr[y] /= matrix.getcolums();
            count = 0;
            ++y;
            while(y != point->y)
                ++y;
        }
        arr[y] += point->value;
        ++count;
    }
    //if (count != 0)
    //arr[y] /= count;
    arr[y] /= matrix.getcolums();
    y = 0;
    for (size_t i = 0; i < matrix.getsize(); ++i)
    {
        point = matrix.get(i);
        while (y != point->y) ++y;
        if (arr[y] < point->value)
            res[y] += point->value;
    }
    return res;
}
