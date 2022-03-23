//
// Created by leyli on 06.10.2021.
//

#include "RandomSelection.h"
const char* InvalidOptions::what() const throw()
{
    return "Invalid Options";
}

const char* Empty::what() const throw()
{
    return "Selection is empty";
}

RandomSelection::RandomSelection(const int n0): selection(nullptr){
    if (n0 < 0)
        throw InvalidOptions();
    n = n0;
    if(!n) {
        return;
    }
    selection = new float[n0];
    for (int i = 0; i < n0; ++i)
        selection[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

}

RandomSelection::RandomSelection(const float *arr, int sz): selection(nullptr){
    if(sz <=0 || arr == nullptr)
        throw InvalidOptions();
    n = sz;
    selection = new float[sz];
    for (int i = 0; i < sz; ++i) {
        if (arr[i] > 1 || arr[i] < 0) {
            delete[] selection;
            throw InvalidOptions();
        }
        selection[i] = arr[i];
    }
}

RandomSelection::RandomSelection(const RandomSelection &r): n(r.n), selection(nullptr){
    if(!n)
        return;
    selection = new float[n];
    memcpy(selection, r.selection, sizeof(float)*n);
}

RandomSelection& RandomSelection::operator =(const RandomSelection& r){
    if(this != &r){
        delete[] selection;
        selection = nullptr;
        n = r.n;
        if(n != 0){
            selection = new float[n];
            memcpy(selection, r.selection, sizeof(float)*n);
        }
    }
    return *this;
}

RandomSelection operator +(const RandomSelection& r1, const RandomSelection& r2){
    RandomSelection tmp(r1.n + r2.n);
    tmp.n = r1.n + r2.n;
    memcpy(tmp.selection, r1.selection, sizeof(float)*r1.n);
    memcpy(tmp.selection + r1.n, r2.selection, sizeof(float)*r2.n);
    return tmp;
}

RandomSelection& RandomSelection::operator !() noexcept{
    for (int i = 0; i < n; ++i) {
        selection[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    }
    return *this;
}

RandomSelection& RandomSelection::operator ++(){
    float* old = selection;
    selection = new float[n+1];
    memcpy(selection, old, sizeof(float)*n);
    selection[n] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    ++n;
    delete[] old;
    return *this;
}

RandomSelection& operator +=(RandomSelection& r, const float f){
    if(f > 1 || f < 0) {
        throw InvalidOptions();
    }
    float* old = r.selection;
    r.selection = new float[r.n+1];
    memcpy(r.selection, old, sizeof(float)*r.n);
    r.selection[r.n] = f;
    ++r.n;
    delete[] old;
    return r;
}

RandomSelection RandomSelection::operator ()(const float a1, const float a2){
    if(a1 < 0 || a1 > 1 || a2 < 0 || a2 > 1)
        throw InvalidOptions();
    int j = 0;
    auto arr = new float[n];
    for (int i = 0; i < n; ++i){
        if(this->selection[i] >=a1 && this->selection[i] <=a2) {
            arr[j++] = selection[i];
        }
    }
    RandomSelection tmp(j);
    memcpy(tmp.selection, arr, sizeof(float)*j);
    tmp.n = j;
    delete[] arr;
    return tmp;
}

RandomSelection& RandomSelection::operator --() noexcept{
    for (int i = 0; i < n; ++i){
        selection[i] = 1 - selection[i];
    }
    return *this;
}

std::ostream& operator <<(std::ostream& s, const RandomSelection& sel){
    if(sel.n == 0)
        s << "Selection is empty";
    for (int i = 0; i < sel.n; ++i){
        s << sel.selection[i] << std::endl;
    }
    std::cout << std::endl;
    return s;
}

float& RandomSelection::operator [](const int ind) const{
    if(ind < 0 || ind >n)
        throw InvalidOptions();
    return selection[ind];
}

float RandomSelection::Average() const {
    if (n == 0)
        throw Empty();
    float res = 0;
    for (int i = 0; i < n; ++i) {
        res += selection[i];
    }
    return res / (float) n;
}
