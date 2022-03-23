//
//  gang.cpp
//  random_selection_game
//
//  Created by Павел on 15.10.2021.
//

#include "gang.hpp"

std::ostream& operator<<(std::ostream& os, const Gang& gang){
    return gang.gang_print(os);
}
    
Gang::Gang(): size(0){}

Gang::Gang(size_t count, std::string&& name){
    size = count;
    street_name = std::move(name);
    chance = RandomSelection(static_cast<int>(count));
    (count == 1) ? heals.create_ptr(new int) : heals.create_ptr(new int[count], count);
    for (size_t i = 0; i < count; ++i){
        heals[i] = Max_heal;
    }
    is_alive = count;
}

Gang::Gang(const Gang& g){
    size = g.size;
    chance = g.chance;
    street_name = g.street_name;
    heals = g.heals;
    is_alive = g.is_alive;
}

Gang& Gang::operator=(const Gang& g){
    if (this == &g) return *this;
    Gang tmp = g;
    std::swap(size, tmp.size);
    std::swap(chance, tmp.chance);
    std::swap(street_name, tmp.street_name);
    std::swap(heals, tmp.heals);
    std::swap(is_alive, tmp.is_alive);
    return *this;
}

Gang::Gang(Gang&& g): size(g.size){
    g.size = 0;
    std::swap(chance, g.chance);
    std::swap(street_name, g.street_name);
    std::swap(heals, g.heals);
    std::swap(is_alive, g.is_alive);
}

Gang& Gang::operator=(Gang&& g){
    if (this == &g) return *this;
    Gang tmp = std::move(g);
    std::swap(size, tmp.size);
    std::swap(chance, tmp.chance);
    std::swap(street_name, tmp.street_name);
    std::swap(heals, tmp.heals);
    std::swap(is_alive, tmp.is_alive);
    return *this;
}

Gang::~Gang() = default;

std::pair<float, int*> Gang::operator[](size_t ind){
    std::pair<float, int*> pair = std::make_pair(chance[static_cast<int>(ind)], &heals[ind]);
    return pair;
}

std::ostream& Gang::gang_print(std::ostream& os) const{
    for (size_t i = 0; i < size; ++i){
        if (static_cast<int>(heals[i]) == 0){
            os << i << " KILLED" << std::endl;
        }else
            os << i << " Hp(" << static_cast<int>(heals[i]) << ")"  << "Ch(" << chance[static_cast<int>(i)] << ")" << std::endl;
    }
    return os;
}


bool Gang::new_selection(){
    try{
        RandomSelection tmp(static_cast<int>(size));
        chance = tmp;
    }catch(std::exception& ba){
        return false;
    }
    return true;
}


//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------

std::string game::make_name() const{
    std::string name;
    while (name.size() == 0){
        std::cout << "\nEnter street name for gang::";
        if (!(std::cin >> name))
            if (std::cin.eof()) throw MLT::Eof_error("Programm will be closed");
    }
    return name;
}

Gang* game::init() const{
    size_t size;
    do{
        std::cout << "Enter gang size::";
        size = MLT::get_num<size_t>();
        if (size == 0) std::cout << "size can't be equal 0";
    } while(size == 0);
    std::string name1 = make_name();
    std::string name2 = make_name();
    while(name1 == name2){
        name2 = make_name();
    }
    
    Gang g1(size, std::move(name1));
    Gang g2(size, std::move(name2));
    
    Gang* coast = new Gang[2];
    coast[0] = std::move(g1);
    coast[1] = std::move(g2);
    
    return coast;
}

bool game::start(){
    Gang* coast = init();
    size_t prev[2];
    while(make_turn(coast, prev)) {}
    std::cout << "The end\nAll units of " << coast[(turn + 1) % 2].street_name << " died\n";
    std::cout << "So " << coast[turn].street_name << " WIIIINNN!!!!\n";
    return true;
}


bool game::make_turn(Gang* coast, size_t prev[]){
    int other = (turn + 1) % 2;
    std::cout << "\n" << coast[turn].street_name << "::\n" << coast[turn];
    std::cout << "\n" << coast[other].street_name << "::\n" << coast[other] << "\n";
    
    std::cout << coast[turn].street_name << " makes move\n";
    
    size_t sz = coast[0].size;
    size_t ind = sz;
    size_t hit_ind = sz;
    
    while (ind >= sz || hit_ind >= sz){
        try{
            std::cout << "Enter number of your men::";
            ind = MLT::get_num<size_t>();
            std::cout << "Enter number of enemy gangs unit::";
            hit_ind = MLT::get_num<size_t>();
        }catch(MLT::Eof_error& er){
            delete [] coast;
            throw;
        }
        
        if (ind >= sz || hit_ind >= sz) continue;
        
        if (prev[turn] == ind && coast[turn].is_alive > 1){
            std::cout << "This unit fought yesterday,so he needs a rest\n";
            ind = sz;
            continue;
        }
        
        auto now = coast[turn][ind];
        auto outher = coast[other][hit_ind];
        
        if (*now.second == 0) {
            hit_ind = sz;
            std::cout << "Your unit has already died\n";
            continue;
        }
        
        if (*outher.second == 0){
            ind = sz;
            std::cout << "Unit you chooce has already died\n";
            continue;
        }
        
        prev[turn] = ind;
        
        if (is_hit(now.first)){
            *outher.second -= Damage;
            if (*outher.second <= 0){
                *outher.second = 0;
                --coast[other].is_alive;
                if (coast[other].is_alive == 0)
                    return false;
            }
        }
    }
    
    turn = other;
    return true;
}


bool game::is_hit(float chance) const{
    uint8_t rand = std::rand() % 100;
    while (chance < 10){
        chance *= 10;
    }
    if (rand <= chance) return true;
    return false;
}
