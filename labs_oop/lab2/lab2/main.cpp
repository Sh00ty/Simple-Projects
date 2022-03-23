//
//  main.cpp
//  lab2
//
//  Created by Павел on 10.09.2021.
//

/*
1) Определить состояние класса.

2) Разработать необходимые конструкторы и методы получения и изменения параметров, определяющих кривую.

3) Вернуть координаты точки принадлежащей циклоиде в декартовой системе координат от угла поворота луча, соединяющего центр порождающего круга с центром направляющей окружности.

4) Вернуть радиусы ограничивающих гипоциклоиду окружностей.

5) Вернуть радиус кривизны от угла поворота луча, соединяющего центр порождающего круга с центром направляющей окружности.

6) Вернуть тип гипоциклоиды (обыкновенная, укороченная, удлиненная).

7) Вернуть секториальную площадь гипоциклоиды относительно начального положения описываемой полярным радиусом.
*/


#include "MyNameSpace.hpp"
#include <Curve_Class.hpp>


std::ostream& operator<<(std::ostream& out, const Hc& hc)
{
    return out << "R::" << hc.get_big_radius() << "  r::" << hc.get_small_radius()<< "  d::" << hc.get_offset() << std::endl;
}

std::ostream& operator<<(std::ostream& out, const point& point)
{
    
    return out << "X::" << point.x << "  Y::"  << point.y << std::endl;
}


bool get_curve(Hc& hc)
{
    double x = 0, y = 0, R = 0, r = 0, d = 0;
    std::cout << "\nEnter R, r, d::";
    if (!MySpace::get_num(R) || !MySpace::get_num(r) || !MySpace::get_num(d)) return false;
    std::cout << "\nEnter x, y::" << std::endl;
    if (!MySpace::get_num(x) || !MySpace::get_num(y)) return false;
    Hc tmp(R, r, d, x, y);
    hc = tmp;
    return true;
}

void print_menu()
{
    std::cout << "\nEnter -1 to exit\n"
                "Enter 0 to create curve\nEnter 1 to get curve parametrs\nEnter 2 to return coordinates by angle\nEnter 3 to print type of hypocycloid\nEnter 4 to get radius of curvature\nEnter 5 to get sector area by angle\nEnter 6 get limitimg circles radius\n::";
}

void main_menu()
{
    print_menu();
    int input;
    Hc hc(0, 0, 0, 0, 0);
    if (!MySpace::get_num(input)) return;
    while(true)
    {
        if (input != -1 && input != 0)
            if (!hc.is_correct())
            {
                std::cout << "Curve is invalid" << std::endl;
                input = 0;
            }
        switch (input)
        {
            case -1: return;
            case 0:
            {
                get_curve(hc);
                break;
            }
            case 1:
            {
                std::cout << hc;
                break;
            }
            case 2:
            {
                double angle;
                std::cout << "\nEnter angle::";
                if (!MySpace::get_num(angle)) break;
                std::cout << hc.get_point_by_angle(angle);
                break;
            }
            case 3:
            {
                hc.what_type();
                break;
            }
            case 4:
            {
                double angle;
                std::cout << "\nEnter angle::";
                if (!MySpace::get_num(angle)) break;
                std::cout << "r = " << hc.get_radius_of_curvature(angle);
                break;
            }
            case 5:
            {
                double angle;
                std::cout << "\nEnter angle::";
                if (!MySpace::get_num(angle)) break;
                std::cout << "area:: " << hc.get_sector_area(angle);
                break;
            }
            case 6:
            {
                std::cout << "R(x), r(y):: " << hc.get_limitimg_circles_radius();
                break;
            }
            default:
            {
                std::cout << "Invalid input";
                break;
            }
        }
        print_menu();
        if (!MySpace::get_num(input)) return;
    }
}

int main()
{
    main_menu();
    return 0;
}

