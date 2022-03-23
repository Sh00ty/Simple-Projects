//
//  Schools.hpp
//  summoning_game
//
//  Created by Павел on 01.11.2021.
//

#ifndef Schools_hpp
#define Schools_hpp


#include <vector>
#include <queue>
#include <memory>
#include <iterator>
#include <list>
#include <fstream>
#include <QPixmap>
#include <QApplication>
#include "graph.hpp"
#include "MyLib.hpp"


enum propetis{
    name = 0,
    school,
    max_damage,
    min_damage,
    max_hp, 
    speed,
    exp_per_unit,
    initiative,
    id,
    min_knowledge,
    exp_per_unit_to_summon,
    lev_hed_rat,
    count,
    hp,
    moral,
    poisoned_long,
    owner,
    exp,
    max_energy,
    accumulation_coefficient,
    energy,
    X,
    Y,
    squads_count
};

enum squad_ID { isquad=0, idist_squad, ipoison_sqaud, iself_heal_sqaud, iSummoner, ichest };

struct base_unit{
    uint8_t name;
    uint8_t school;
    
    uint8_t max_damage;
    uint8_t min_damage;
    
    uint8_t max_hp;
    
    uint8_t speed;
        
    uint8_t exp_per_unit;
    
    uint8_t initiative;
    
    base_unit(const std::vector<int>& prop){
        name = prop[propetis::name];
        school = prop[propetis::school];
        min_damage = prop[propetis::min_damage];
        max_damage = prop[propetis::max_damage];
        max_hp = prop[propetis::max_hp];
        speed = prop[propetis::speed];
        exp_per_unit = prop[propetis::exp_per_unit];
        initiative = prop[propetis::initiative];
    }
};


namespace schools{

struct summon_property{
    uint8_t id;
    
    uint8_t min_knowledge;
    // exp to unit to summon
    uint8_t exp_per_unit_to_summon;
    // headncount ratio to summon by level
    uint8_t lev_hed_rat;
    
    std::shared_ptr<base_unit> bu;
};


struct school_table{
    using Map = MyUnorderedMap<int8_t, std::list<summon_property>>;
    
    Map map;

    std::vector<std::string> schools_names;
    std::vector<std::string> units_names;
    std::vector<QPixmap> units_images;
    
    school_table() = default;
    
    school_table(std::ifstream& in);

    school_table(std::ifstream&& in);
    
    summon_property* get(uint8_t school_name, uint8_t unit_name) noexcept;
    
    size_t school_count() const noexcept;
    
    size_t unit_count_in_school(int8_t name) const noexcept;
    
    void read_schools_from(std::ifstream& in);
    
    void read_names_from(std::ifstream& in);
    
    summon_property read_unit_from(std::ifstream& in);
    
    void load_images();
};

extern school_table sc_table;


};


#endif /* Schools_hpp */
