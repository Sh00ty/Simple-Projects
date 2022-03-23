//
//  Schools.cpp
//  summoning_game
//
//  Created by Павел on 01.11.2021.
//

#include "Schools.hpp"

using namespace schools;

//extern school_table sc_table;

//--------------------------------------------------------------------- table_funcs

summon_property* school_table::get(uint8_t school_name, uint8_t unit_name) noexcept{
    auto f = map.find(school_name);
    
    for (auto& i : f->second){
        if (i.bu->name == unit_name){
            return &i;
        }
    }
    return nullptr;
}


size_t school_table::school_count() const noexcept{
    return schools_names.size();
}


size_t school_table::unit_count_in_school(int8_t name) const noexcept{
    return map.find(name)->second.size();
}


//--------------------------------------------------------------------- files_funcs


void school_table::read_names_from(std::ifstream& in){
    int count = 0;
    in >> count;
    if (in.bad() || count == 0){
        in.close();
        throw std::runtime_error("file has been broken");
    }
    for (int i = 0; i < count; ++i){
        std::string name;
        in >> name;
        if (in.bad()){
            in.close();
            throw std::runtime_error("file has been broken");
        }
        units_names.push_back(name);
    }
}


void school_table::read_schools_from(std::ifstream& in){
    int count = 0;
    in >> count;
    if (in.bad() || count == 0){
        in.close();
        throw std::runtime_error("file has been broken");
    }
    for (int i = 0; i < count; ++i){
        std::string name;
        in >> name;
        if (in.bad()){
            in.close();
            throw std::runtime_error("file has been broken");
        }
        schools_names.push_back(name);
        std::cout << "\nschool " <<  name;
        std::cout << map.insert(std::make_pair(schools_names.size() - 1, std::list<summon_property>())).second;
    }

}


summon_property school_table::read_unit_from(std::ifstream& in){
    std::vector<int> prop;
    for (int i = 0; i < propetis::lev_hed_rat + 1; ++i){
        int unit_prop;
        in >> unit_prop;
        if (in.bad()){
            in.close();
            throw std::runtime_error("file has been broken");
        }
        prop.push_back(unit_prop);
    }
    summon_property sp;
    sp.bu = std::make_shared<base_unit>(prop);
    sp.id = prop[id];
    sp.min_knowledge = prop[min_knowledge];
    sp.exp_per_unit_to_summon = prop[exp_per_unit_to_summon];
    sp.lev_hed_rat = prop[lev_hed_rat];
    map[sp.bu->school].push_back(sp);
    return sp;
}


void school_table::load_images(){
    units_images.reserve(units_names.size());
    units_images.emplace_back("/Users/pavelshykov/Desktop/Qtest/test/images/vamp_summoner.png");
    units_images.emplace_back("/Users/pavelshykov/Desktop/Qtest/test/images/knight_summoner.jpg");
    
    units_images.emplace_back("/Users/pavelshykov/Desktop/Qtest/test/images/scelet.png");
    units_images.emplace_back("/Users/pavelshykov/Desktop/Qtest/test/images/zombie.png");
    units_images.emplace_back("/Users/pavelshykov/Desktop/Qtest/test/images/vampire.png");
    
    units_images.emplace_back("/Users/pavelshykov/Desktop/Qtest/test/images/hech.png");
    units_images.emplace_back("/Users/pavelshykov/Desktop/Qtest/test/images/cerber.png");
    units_images.emplace_back("/Users/pavelshykov/Desktop/Qtest/test/images/sucub.png");
    
    units_images.emplace_back("/Users/pavelshykov/Desktop/Qtest/test/images/assasin.png");
    units_images.emplace_back("/Users/pavelshykov/Desktop/Qtest/test/images/hydra.png");
    units_images.emplace_back("/Users/pavelshykov/Desktop/Qtest/test/images/dark_dragon.png");
}


school_table::school_table(std::ifstream&& in){
    read_schools_from(in);
    read_names_from(in);
    for (int i = 0; i < 9; ++i)
        read_unit_from(in);
}


school_table::school_table(std::ifstream& in){
    read_schools_from(in);
    read_names_from(in);
    for (int i = 0; i < 9; ++i)
        read_unit_from(in);
    load_images();
}
