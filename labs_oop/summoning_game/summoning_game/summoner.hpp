//
//  summoner.hpp
//  summoning_game
//
//  Created by Павел on 31.10.2021.
//

#ifndef summoner_hpp
#define summoner_hpp

#include <QtGui>
#include "Schools.hpp"
#include <QVBoxLayout>



enum _interaction{enothing = 0, echest, efight};
class squad;
struct Summoner;
using shared_ptr = std::shared_ptr<squad>;

struct position{
    std::pair<int, int> limits;
    
    position(std::pair<int, int> lim){
        limits = lim;
    }
    
    int operator()(int x, int y){
        return x + y * limits.first;
    }
    
    int operator()(std::pair<int, int> point){
        return point.first + point.second * limits.first;
    }
};

bool is_belong_to_area(std::pair<int, int> x, std::pair<int, int> lim2);

struct object{
    int X;
    int Y;
    static std::shared_ptr<object> Create_object(uint8_t id);
    virtual _interaction interaction(std::shared_ptr<Summoner>& s) = 0;
    virtual QVBoxLayout* print_stats() = 0;
    virtual ~object() {}
};

//--------------------------------------------------------------------- squads classes

class squad: public object, public std::enable_shared_from_this<squad>{
protected:
    virtual bool __attack(squad& s);
    int create_damage(const squad& s);
public:
    std::shared_ptr<base_unit> base;
    
    int16_t count = 0;
    
    int32_t hp = 0;
    
    int8_t moral = 0;
    
    uint8_t poisoned_long = 0;
    
    int x;
    int y;
    
    std::shared_ptr<Summoner> owner;
    
    static shared_ptr create_squad(uint8_t id);
    
    void make_by_vec(const std::vector<int>& prop);
    
    void make_by_short_vec(const std::vector<int>& prop);
    
    bool attack(squad& s);
    
    void moral_balance();
    
    void poison_effect();
    
    _interaction interaction(std::shared_ptr<Summoner>& s) override;
    
    QVBoxLayout* print_stats() override;
    
    ~squad() override = default;
    
protected:
    template <typename T>
    std::shared_ptr<T> shared_from_base(){
        return std::static_pointer_cast<T>(shared_from_this());
    }
};


class poison_sqaud: public squad{
    
    void poison(squad& s, int damage);
    
    bool __attack(squad& s) override;
public:
    poison_sqaud() = default;
    
    ~poison_sqaud() override = default;
};


class dist_squad: public squad{
    
    bool __attack(squad& s) override;
public:
    dist_squad() = default;
    
    ~dist_squad() override = default;
};


class self_heal_sqaud: public squad{
    
    void self_heal(int damage);
    
    bool __attack(squad& s) override;
    
public:
    self_heal_sqaud() = default;
    
    ~self_heal_sqaud() override = default;
};


//--------------------------------------------------------------------- summoner

struct Summoner: public squad{
    
    uint16_t exp;
    
    int16_t max_energy;
    uint16_t accumulation_coefficient;
    int16_t energy;
    
    std::vector<shared_ptr> my_squads;
    std::vector<uint8_t> knowledge;
    
    Summoner() = default;
    
    void make_by_vec_sum(const std::vector<int>& prop);
    
    std::vector<int> read_from(std::ifstream& in);
    
    bool school_up(uint8_t name, uint8_t diff);
    
    void energy_accumulation();
    
    bool summon(uint8_t school_name, uint8_t name, uint16_t count);
    
    void killed(shared_ptr& unit);
    
    _interaction interaction(std::shared_ptr<Summoner>& s) override;
    
    QVBoxLayout* print_stats() override;
    
    ~Summoner() override = default;
};



//--------------------------------------------------------------------- other



struct chest: object{
    int exp_count = 0;
    int energy = 0;
    // 1 - school_name, 2 - diff
    std::pair<int, int> school_up = {0, 0};
    
    _interaction interaction(std::shared_ptr<Summoner>& s) override;
    
    QVBoxLayout* print_stats() override;
    
    void to_empty();
    
    ~chest() override = default;
    
    void make_by_vec(const std::vector<int>& prop);
};


#endif /* summoner_hpp */
