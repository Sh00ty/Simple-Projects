//
//  MyNameSpace.hpp
//  cpp_test
//
//  Created by Павел on 06.09.2021.
//

#ifndef MyNameSpace_hpp
#define MyNameSpace_hpp

#include <iostream>


namespace MySpace
{
    template<typename T>
    bool get_num(T& num)
    {
        while(!(std::cin >> num))
        {
            std::cout << std::endl << "Enter failed, try again\n::";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (std::cin.eof())
            {
                std::cout << std::endl << "programm will close!!!";
                return false;
            }
        }
        return true;
    }
}
#endif /* MyNameSpace_hpp */
