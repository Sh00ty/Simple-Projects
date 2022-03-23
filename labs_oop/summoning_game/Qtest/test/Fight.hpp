//
//  fight.hpp
//  test
//
//  Created by Павел on 08.12.2021.
//

#ifndef fight_hpp
#define fight_hpp

#include "fight_button.hpp"
#include <QWidget>
#include <QDialog>
#include <memory>
#include <iostream>
#include <queue>
#include <vector>


static auto sq_cmp = [] (const std::shared_ptr<squad>& left, const std::shared_ptr<squad>& right) { return left->base->initiative < right->base->initiative; };

class Fight: public QDialog{
    Q_OBJECT;
    
    std::vector<FightButton*> land;
    std::pair<int, int> size = {10, 6};
    position pos;
    
    std::shared_ptr<Summoner> s1;
    size_t co1 = 0;
    std::shared_ptr<Summoner> s2;
    size_t co2 = 0;
    std::priority_queue<std::shared_ptr<squad>, std::vector<std::shared_ptr<squad>>, decltype(sq_cmp)> queue = std::priority_queue<std::shared_ptr<squad>, std::vector<std::shared_ptr<squad>>, decltype(sq_cmp)>(sq_cmp);
    
    std::shared_ptr<squad> now;
    
    bool started = false;
public:
    
    Fight(std::shared_ptr<Summoner>& s1, std::shared_ptr<Summoner>& s2, QWidget* parent = nullptr);
    
    void fill();
        
    void make_land();
    
    std::shared_ptr<object> stand(std::shared_ptr<squad>& s, std::pair<int, int> cor);
    
    static bool can_move2(int x, int y, int speed, std::vector<std::shared_ptr<object>>&& area, std::pair<int, int> limits, std::pair<int, int> dest);
    
    void move_pers(std::shared_ptr<squad>& s, std::pair<int, int> cords);
    
    void find_near_place(std::shared_ptr<squad>& sq, int position);
    
    void turn();
    
    void kill(std::shared_ptr<squad>& s);
    
    bool can_move(std::pair<int, int> start, int speed, std::pair<int, int> dest, bool ll = false);
    
    std::pair<int, int> target_search(shared_ptr& unit);
protected:
    void paintEvent(QPaintEvent *event) override;
    
public slots:
    void slt();

};

#endif /* fight_hpp */
