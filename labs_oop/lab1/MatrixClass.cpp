//
//  MatrixClass.cpp
//  cpp_test
//
//  Created by Павел on 07.09.2021.
//

#include "MatrixClass.hpp"

Matrix::Matrix() = default;

Matrix::Matrix(size_t size, size_t colums, size_t rows): size(size), colums(colums), rows(rows)
{
    if (size == 0)
    {
        this->size = 0;
        return;
    }
    mat = new point[size];
}

Matrix::Matrix(const Matrix& matrix): Matrix(matrix.size, matrix.colums, matrix.rows)
{
    count = matrix.count;
    memcpy(mat, matrix.mat, matrix.size * sizeof(point));
}



Matrix::~Matrix()
{
    delete[] mat;
}


Matrix& Matrix::operator=(Matrix Matrix)
{
    std::swap(colums, Matrix.colums);
    std::swap(rows, Matrix.rows);
    std::swap(size, Matrix.size);
    std::swap(count, Matrix.count);
    std::swap(mat, Matrix.mat);
    return *this;
}


void Matrix::del()
{
    if (count == 0) return;
    mat[count].x = -1;
    mat[count].y = -1;
    mat[count].value = -1;
    --count;
}


void Matrix::add(int x, int y, int val)
{
    if (count == size) return;
    mat[count].x = x;
    mat[count].y = y;
    mat[count].value = val;
    ++count;
}

void Matrix::print() const
{
    for (size_t i = 0; i < count; ++i)
    {
        std::cout << "\nx:: " << (mat[i].x) << " y:: " << (mat[i].y) << " value:: " << (mat[i].value) << std::endl;
    }
}


Matrix::point* Matrix::get(size_t index) const
{
    if (count <= index) return nullptr;
    return mat + index;
}

// can be done by only new-delete, but this looks like better
void Matrix::resize()
{
    Matrix tmp(count, colums, rows);
    memcpy(tmp.mat, mat, count * sizeof(point));
    std::swap(tmp.mat, this->mat);
    size = count;
    //return *this;
}


size_t Matrix::getsize() const
{
    return size;
}


size_t Matrix::getcolums() const
{
    return colums;
}


size_t Matrix::getrows() const
{
    return rows;
}
