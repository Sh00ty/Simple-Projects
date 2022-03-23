//
//  main.cpp
//  summoning_game
//
//  Created by Павел on 31.10.2021.
//

#include "game.hpp"

int main() {
    std::ifstream in("/Users/pavelshykov/Desktop/labs/summoning_game/units_propertis");
    schools::sc_table = schools::school_table(in);
    for (auto i : schools::sc_table.units_names)
        std::cout << i << std::endl;
    for (auto i : schools::sc_table.schools_names)
        std::cout << i << std::endl;
    return 0;
}
