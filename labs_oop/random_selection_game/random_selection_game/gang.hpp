//
//  gang.hpp
//  random_selection_game
//
//  Created by Павел on 15.10.2021.
//

#ifndef gang_hpp
#define gang_hpp

#include <iostream>
#include <cmath>
#include <MyLib.hpp>
#include "../lab3-main/RandomSelection.h"


static const int Damage = 100;
static const int Max_heal = 100;

struct Gang{
    
    size_t size;
    std::string street_name;
    MLT::SmartPtr<int> heals;
    RandomSelection chance;
    size_t is_alive;
    
    Gang();
    Gang(size_t count, std::string&& name);
    ~Gang();
    
    Gang(const Gang& g);
    Gang& operator=(const Gang& g);
    
    Gang(Gang&& g);
    Gang& operator=(Gang&& g);

    std::pair<float, int*> operator[](size_t ind);
    
    std::ostream& gang_print(std::ostream& os = std::cout) const;
    
    bool new_selection();
};

std::ostream& operator<<(std::ostream& os, const Gang& gang);


struct game{
    int turn = 0;
    
    std::string make_name() const;
    
    Gang* init() const;
    
    bool start();
    
    bool make_turn(Gang* coast, size_t prev[]);
    
    bool is_hit(float chance) const;
};



#endif /* gang_hpp */
