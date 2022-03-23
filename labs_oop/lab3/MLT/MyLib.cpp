//
//  MyLib.cpp
//  MySpace
//
//  Created by Павел on 21.09.2021.
//

#include "MyLib.hpp"
namespace MLT{

bool is_equal(double r, double d) noexcept{
    return fabs(r - d) < std::numeric_limits<double>::epsilon();
}
    
}
