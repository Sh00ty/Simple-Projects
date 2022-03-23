//
// Created by leyli on 06.10.2021.
//

#ifndef RandomSelection_h
#define RandomSelection_h
#pragma ones

#include <iostream>
#include <cstring>
#include <random>
#include <ctime>

class InvalidOptions: public std::exception {
    virtual const char* what() const throw();
};

class Empty: public std::exception {
    virtual const char* what() const throw();
};

class RandomSelection {
private:
    int n;
    float* selection;
public:
    RandomSelection(int n0 = 0);
    RandomSelection(const float* arr, int sz); //инициализация массивом чисел от 0 до 1
    RandomSelection(const RandomSelection& r);
    ~RandomSelection(){delete[] selection;}
    RandomSelection& operator =(const RandomSelection& r);
    friend RandomSelection operator +(const RandomSelection& r1, const RandomSelection& r2); //инициализация суммой целых чисел  и/или выборок
    int getn() const noexcept{return n;}
    float& operator [](int ind) const; //выделение элемента выборки по индексу
    RandomSelection& operator !() noexcept; //обновление выборик
    RandomSelection& operator ++(); //дополнение выборки случайным числом
    friend RandomSelection& operator +=(RandomSelection& r, float f); //дополнение выборки случайным числом, сгенерированным ранее
    RandomSelection operator ()(float a1, float a2); //выделение новой выборки из старой (числа принадл. диапазону)
    RandomSelection& operator --() noexcept; //дополнение до 1
    friend std::ostream& operator <<(std::ostream& s, const RandomSelection& sel); //вывод выборки
    float Average() const; //среднее арифметическое
};

#endif //LAB3C_RANDOMSELECTION_H
