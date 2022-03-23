//
//  main.cpp
//  lab3
//
//  Created by Pavel Shlykov B20-515 on 25.09.2021.
//

#include <iostream>
#include "MyLib.hpp"
#include "StaticFunc.hpp"

using namespace lab3S;


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
    StaticFunction sf;
    try{
        std::cin >> sf;
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
                    std::cout << sf;
                    break;
                }
                case 1:
                {
                    std::cout << "x:";
                    double x = MLT::get_num<double>();
                    std::cout << "y:";
                    double y = MLT::get_num<double>();
                    point p(x, y);
                    sf.add(p);
                    break;
                }
                case 2:
                {
                    std::cout << "Enter X:: ";
                    double x = MLT::get_num<double>();
                    std::cout << "f(" << x << ") = " << sf.value(x) << std::endl;
                    break;
                }
                case 3:
                {
                    char inp = sf.type();
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
                }
                case 4:
                {
                    std::cout << "Max value is:: " << sf.max() << std::endl;
                    break;
                }
                case 5:
                {
                    std::cout << "Min value is:: " << sf.min() << std::endl;
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
