//
//  summoner.cpp
//  summoning_game
//
//  Created by Павел on 31.10.2021.
//

#include "Summoner.hpp"
#include "game.hpp"
#include "Fight.hpp"
#include <QWidget>
#include <QLabel>
#include <QString>
#include <QHBoxLayout>

schools::school_table schools::sc_table;


//--------------------------------------------------------------------- attack

void Summoner::killed(shared_ptr& unit){
    for (auto i = my_squads.begin(); i != my_squads.end(); ++i){
        if ((*i)->base->name == unit->base->name){
            my_squads.erase(i);
            return;
        }
    }
}


int squad::create_damage(const squad& s){
    int damage = (s.base->max_damage + s.base->min_damage)/2 + s.moral;
    if (damage > s.base->max_damage) damage = s.base->max_damage;
    if (damage < s.base->min_damage) damage = s.base->min_damage;
    return damage;
}


void self_heal_sqaud::self_heal(int damage){
    count += damage / (2 * max_hp);
    hp += damage;
}


void poison_sqaud::poison(squad& s, int damage){
    s.poisoned_long = static_cast<int>(damage * 0.2);
}


void squad::poison_effect(){
    if (poisoned_long == 0) return;
    --poisoned_long;
    hp -= poisoned_long;
    if (hp <= 0) {
        count = 0;
        hp = 0;
        return;
    }
    count = hp / base->max_hp + (hp % base->max_hp > 0 ? 1 : 0);
}

bool squad::attack(squad& s){
    if (!__attack(s)) return false;
    if (s.count > 0) s.__attack(*this);
    return true;
}

bool squad::__attack(squad& s){
    
    if (x != s.x && y != s.y) return false;
    if (abs(x - s.x) != 1 && abs(y - s.y) != 1) return false;
    
    int damage = create_damage(*this);
    damage *= count;
    s.hp -= damage;
    if (s.hp <= 0){
        s.hp = 0;
        s.count = 0;
        ++moral;
        return true;
    }
    int prev_count = s.count;
    s.count = s.hp / s.base->max_hp + (s.hp % s.base->max_hp > 0 ? 1 : 0);
    if (prev_count - s.count > 0){
        ++moral;
        --s.moral;
    }
    return true;
}

bool self_heal_sqaud::__attack(squad& s){
    
    if (x != s.x && y != s.y) return false;
    if (abs(x - s.x) != 1 && abs(y - s.y) != 1) return false;
    
    int damage = create_damage(*this);
    damage *= count;
    s.hp -= damage;
    if (s.hp <= 0){
        s.hp = 0;
        s.count = 0;
        ++moral;
        --s.moral;
        return true;
    }
    int prev_count = s.count;
    s.count = s.hp / s.base->max_hp + (s.hp % s.base->max_hp > 0 ? 1 : 0);
    self_heal(damage);
    if (prev_count - s.count > 0){
        ++moral;
        --s.moral;
    }
    return true;
}

bool dist_squad::__attack(squad& s){
    
    int damage = create_damage(*this);
    damage *= count;
    s.hp -= damage;
    if (s.hp <= 0){
        s.hp = 0;
        s.count = 0;
        ++moral;
        return true;
    }
    int prev_count = s.count;
    s.count = s.hp / s.base->max_hp + (s.hp % s.base->max_hp > 0 ? 1 : 0);
    if (prev_count - s.count > 0){
        ++moral;
        --s.moral;
    }
    return true;
}

bool poison_sqaud::__attack(squad& s){
    
    if (x != s.x && y != s.y) return false;
    if (abs(x - s.x) != 1 && abs(y - s.y) != 1) return false;
    
    int damage = create_damage(*this);
    damage *= count;
    s.hp -= damage;
    if (s.hp <= 0){
        s.hp = 0;
        s.count = 0;
        ++moral;
        return true;
    }
    int prev_count = s.count;
    s.count = s.hp / s.base->max_hp + (s.hp % s.base->max_hp > 0 ? 1 : 0);
    poison(s, damage);
    if (prev_count - s.count > 0){
        ++moral;
        --s.moral;
    }
    return true;
}

//--------------------------------------------------------------------- moves

bool is_belong_to_area(std::pair<int, int> x, std::pair<int, int> lim2){
    return (x.first >= 0 && x.first < lim2.first && x.second >= 0 && x.second < lim2.second);
}

//template<>
//struct std::hash<std::pair<int, int>>{
//    std::size_t operator()(const std::pair<int, int>& s) const noexcept{
//        return static_cast<size_t>(s.first * 3 + s.second * 37);
//    }
//};


//--------------------------------------------------------------------- porops

void squad::moral_balance(){
    if (moral > 0) --moral;
    if (moral < 0) ++moral;
}


bool Summoner::school_up(uint8_t name, uint8_t diff){
    if (diff > exp) return false;
    exp -= diff;
    knowledge[name] += diff;
    return true;
}


void Summoner::energy_accumulation(){
    energy = (energy + accumulation_coefficient > max_energy ? max_energy : energy + accumulation_coefficient);
}


bool Summoner::summon(uint8_t school_name, uint8_t nname, uint16_t ucount){
    const schools::summon_property* prop = schools::sc_table.get(school_name, nname);
    
    for (auto& i : my_squads){
        if (i->base->name == nname){
            i->count+= ucount;
            i->hp += ucount * i->base->max_hp;
            energy -= ucount * prop->exp_per_unit_to_summon;
            return;
        }
    }
    
    auto sq = create_squad(prop->id);
    sq->base = prop->bu;
    sq->count = ucount;
    sq->owner = owner;
    sq->hp = sq->base->max_hp * ucount;
    sq->moral = 0;
    sq->poisoned_long = 0;
    sq->owner = shared_from_base<Summoner>();
    
    energy -= ucount * prop->exp_per_unit_to_summon;
    my_squads.push_back(sq);
    return true;
}


shared_ptr squad::create_squad(uint8_t id){
    shared_ptr p;
    switch (id) {
        case isquad:
            p.reset(new squad);
            break;
        case idist_squad:
            p.reset(new dist_squad);
            break;
        case ipoison_sqaud:
            p.reset(new poison_sqaud);
            break;
        case iself_heal_sqaud:
            p.reset(new self_heal_sqaud);
            break;
        case iSummoner:
            p.reset(new Summoner);
            break;
        default:
            assert(false);
    }
    return p;
}


std::shared_ptr<object> object::Create_object(uint8_t id){
    std::shared_ptr<object> p;
    switch (id) {
        case isquad:
            p.reset(new squad);
            break;
        case idist_squad:
            p.reset(new dist_squad);
            break;
        case ipoison_sqaud:
            p.reset(new poison_sqaud);
            break;
        case iself_heal_sqaud:
            p.reset(new self_heal_sqaud);
            break;
        case iSummoner:
            p.reset(new Summoner);
            break;
        case ichest:
            p.reset(new chest);
            break;
        default:
            assert(false);
    }

    return p;
}


void Summoner::make_by_vec_sum(const std::vector<int>& prop){
    make_by_vec(prop);
    exp = prop[propetis::exp];
    max_energy = prop[propetis::max_energy];
    accumulation_coefficient = prop[propetis::accumulation_coefficient];
    energy = prop[propetis::energy];
    X = prop[propetis::X];
    Y = prop[propetis::Y];
    
    for (size_t i = propetis::Y; i < prop.size(); ++i){
        knowledge.push_back(prop[i]);
    }
}


void squad::make_by_vec(const std::vector<int>& prop){
    base = std::make_shared<base_unit>(prop);
    count = prop[propetis::count];
    hp = prop[propetis::hp];
    moral = prop[propetis::moral];
    poisoned_long = prop[propetis::poisoned_long];
}


void squad::make_by_short_vec(const std::vector<int>& prop){
    base = schools::sc_table.get(prop[0], prop[1])->bu;
    count = prop[2];
    hp = prop[3];
    moral = prop[4];
    // delete prop 5
    poisoned_long = prop[6];
    X = prop[7];
    Y = prop[8];
    x = prop[9];
    y = prop[10];
}


void chest::to_empty(){
    exp_count = 0;
    energy = 0;
    school_up = {0, 0};
}


void chest::make_by_vec(const std::vector<int>& prop){
    exp_count = prop[0];
    energy = prop[1];
    school_up.first = prop[2];
    school_up.second = prop[3];
}


//--------------------------------------------------------------------- files_funcs

std::vector<int> Summoner::read_from(std::ifstream& in){
    std::vector<int> prop;
    for (int i = 0; i < propetis::Y; ++i){
        int a;
        in >> a;
        if (in.bad()){
            in.close();
            throw std::runtime_error("file has been broken");
        }
        prop.push_back(a);
    }
    // knowledges
    for (size_t i = 0; i < schools::sc_table.school_count(); ++i){
        int a;
        in >> a;
        if (in.bad()){
            in.close();
            throw std::runtime_error("file has been broken");
        }
        prop.push_back(a);
    }
    return prop;
}


//--------------------------------------------------------------------- interaction

_interaction squad::interaction(std::shared_ptr<Summoner>& s){
    std::shared_ptr<Summoner> tmp = std::dynamic_pointer_cast<Summoner>(create_squad(iSummoner));
    std::vector<int> prop(20, 0);
    tmp->base = std::make_shared<base_unit>(prop);
    tmp->base->name = uint8_t(-1);
    tmp->my_squads.push_back(shared_from_this());
    tmp->count = tmp->my_squads.size();
    owner = tmp;
    Fight f(s, tmp);
    f.exec();
    return efight;
}


_interaction Summoner::interaction(std::shared_ptr<Summoner>& s){
    std::shared_ptr<Summoner> tmp = shared_from_base<Summoner>();
    Fight f(s,tmp);
    f.exec();
    return efight;
}


_interaction chest::interaction(std::shared_ptr<Summoner>& s){
    s->knowledge[school_up.first] += school_up.second;
    school_up.second = 0;
    s->exp += exp_count;
    exp_count = 0;
    int prev_eng = s->energy;
    s->energy = (s->energy + energy > s->max_energy ? s->max_energy : s->energy + energy);
    energy = s->energy - prev_eng;
    return echest;
}


//--------------------------------------------------------------------- print_stats

QLabel* make_label(int prop, const std::string& str){
    auto lab = new QLabel;
    auto str_f = QString(str.c_str()) + QString::number(prop);
    lab->setText(str_f);
    return lab;
}



QVBoxLayout* chest::print_stats(){
    using namespace schools;
    auto Lay = new QVBoxLayout;
    
    auto info = new QLabel;
    info->setText("Chest");
    Lay->addWidget(info);
    
    Lay->addWidget(make_label(exp_count, std::string("expirience count is ")));
    Lay->addWidget(make_label(energy, std::string("energy count is ")));
    
    auto lab_su = new QLabel;
    auto str_su = QString(sc_table.schools_names[school_up.first].c_str()) + QString(" up on ") + QString::number(school_up.second);
    lab_su->setText(str_su);
    Lay->addWidget(lab_su);
    
    return Lay;
}


QVBoxLayout* squad::print_stats(){
    using namespace schools;
    auto Lay = new QVBoxLayout;
    auto info = new QLabel;
    info->setText(sc_table.units_names[base->name].c_str());
    Lay->addWidget(info);
   
    
    Lay->addWidget(make_label(count, std::string("count is ")));
    Lay->addWidget(make_label(hp, std::string("hp ")));
    Lay->addWidget(make_label(moral, std::string("moral is ")));
    Lay->addWidget(make_label(poisoned_long, std::string("poisoned long is ")));
    return Lay;
}

QVBoxLayout* Summoner::print_stats(){
    using namespace schools;
    auto Lay = new QVBoxLayout;
    
    Lay->addWidget(make_label(count, std::string("count is ")));
    Lay->addWidget(make_label(hp, std::string("hp ")));
    Lay->addWidget(make_label(moral, std::string("moral is ")));
    Lay->addWidget(make_label(poisoned_long, std::string("poisoned long is ")));
    
    Lay->addWidget(make_label(exp, std::string("expirience is ")));
    Lay->addWidget(make_label(energy, std::string("energy count is ")));
    Lay->addWidget(make_label(max_energy, std::string("max energy count is ")));
    
    
    QHBoxLayout* knowledges = new QHBoxLayout;
    for (size_t i = 0; i < knowledge.size(); ++i){
        knowledges->addWidget(make_label(knowledge[i], sc_table.schools_names[i] + " "));
    }
    
    Lay->addItem(knowledges);
    
    if (!my_squads.empty()){
        QVBoxLayout* squads = new QVBoxLayout;
        for (auto& i : my_squads){
            squads->addWidget(make_label(i->count, sc_table.units_names[i->base->name] + " "));
        }
        Lay->addItem(squads);
    }
    return Lay;
}

