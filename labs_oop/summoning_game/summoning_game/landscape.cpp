//
//  landscape.cpp
//  summoning_game
//
//  Created by Павел on 03.11.2021.
//

#include "landscape.hpp"
#include "menu.hpp"
#include "Schools.hpp"

landscape::landscape(std::vector<std::ifstream>& in){
    for (size_t i = 0; i < summoners.size(); ++i){
        summoners[i] = object::Create_object(iSummoner);
        auto p = std::dynamic_pointer_cast<Summoner>(summoners[i]);
        p->make_by_vec_sum(p->read_from(in[i]));
    }
}


