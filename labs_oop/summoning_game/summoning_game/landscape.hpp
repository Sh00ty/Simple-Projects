//
//  landscape.hpp
//  summoning_game
//
//  Created by Павел on 03.11.2021.
//

#ifndef landscape_hpp
#define landscape_hpp


#include "Summoner.hpp"
#include <array>

auto initiative_cmp = [](shared_ptr& left, shared_ptr& right) { return left->base->initiative < right->base->initiative; };

struct landscape{
    static const int summoner_count = 2;
    std::array<std::shared_ptr<object>, summoner_count> summoners;
    
    landscape() = default;
    
    landscape(std::vector<std::ifstream>& in);
    
};


struct Win_Except: std::exception{
    int winner;
    Win_Except(int name){
        winner = name;
    }
};


struct Save_Except: std::exception{
    Save_Except() = default;
};

#endif /* landscape_hpp */
