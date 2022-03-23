//
//  main.cpp
//  DynamicFunc
//
//  Created by Pavel Shlykov B20-515 on 01.10.2021.
//

#include <iostream>
#include <MyLib.hpp>
#include <iterator>
#include "DynamicFunc.hpp"

using namespace lab3D;


void print_menu(){
    std::cout << "\nEnter -1 to exit" << std::endl
              << "Enter 0 to print functions points" << std::endl
              << "Enter 1 to add a new point" << std::endl
              << "Enter 2 to show value in any point" << std::endl
              << "Enter 3 to show type of function" << std::endl
              << "Enter 4 to show maximum" << std::endl
              << "Enter 5 to show minimum" << std::endl
              << "GO GO GO::";
}


int main(){
    DynamicFunction df;
    try{
        std::cout << "enter count of points:: ";
        size_t size = MLT::get_num<size_t>();
        for (size_t i = 0; i < size; ++i){
            point p;
            std::cout << "enter x:: ";
            p.x = MLT::get_num<double>();
            std::cout << "enter y:: ";
            p.y = MLT::get_num<double>();
            df.add(p);
        }
    }catch(MLT::Eof_error& ba){
        std::cout << ba.what() << std::endl;
        return 2;
    }catch(std::exception& ba){
        std::cout << ba.what() << std::endl;
    }
    while(true){
        try{
            print_menu();
            int ans = MLT::get_num<int>();
            switch (ans) {
                case -1:
                {
                    return 0;
                }
                case 0:
                {
                    std::cout << df;
                    break;
                }
                case 1:
                {
                    std::cout << "x:";
                    double x = MLT::get_num<double>();
                    std::cout << "y:";
                    double y = MLT::get_num<double>();
                    df.add(point(x, y));
                    break;
                }
                case 2:
                {
                    std::cout << "Enter X:: ";
                    double x = MLT::get_num<double>();
                    std::cout << "f(" << x << ") = " << df.value(x) << std::endl;
                    break;
                }
                case 3:
                {
                    char inp = df.type();
                    switch (inp)
                    {
                        case 1:
                        {
                            std::cout << "function increases" << std::endl;
                            break;
                        }
                        case 0:
                        {
                            std::cout << "function isn't monotonous" << std::endl;
                            break;
                        }
                        case -1:
                        {
                            std::cout << "function decreases" << std::endl;
                            break;
                        }
                        default:
                            std::cout << "function is constant" << std::endl;
                    }
                    break;
                }
                case 4:
                {
                    std::cout << "Max value is:: " << df.max() << std::endl;
                    break;
                }
                case 5:
                {
                    std::cout << "Min value is:: " << df.min() << std::endl;
                    break;
                }
                default:
                {
                    std::cout << "Input is invalid, try again" << std::endl;
                    break;
                }
            }
        }catch(MLT::Eof_error& ex){
            std::cout << ex.what() << std::endl;
            return 2;
        }catch(std::exception& ex){
            std::cout << ex.what() << std::endl;
        }
    }
    
    return 0;
}

