//
//  main.cpp
//  random_selection_game
//
//  Created by Павел on 15.10.2021.
//

#include "gang.hpp"

int main(int argc, const char * argv[]) {
    try{
        game g;
        g.start();
    }catch(std::exception& ba){
        std::cout << ba.what() << std::endl;
    }
    return 0;
}

