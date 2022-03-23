//
//  fight.cpp
//  test
//
//  Created by Павел on 08.12.2021.
//

#include "Fight.hpp"
#include "game.hpp"
#include "Warea.hpp"
#include "menu.hpp"
#include "Summoner.hpp"
    

//--------------------------------------------------------------------- constructors

Fight::Fight(std::shared_ptr<Summoner>& s1, std::shared_ptr<Summoner>& s2, QWidget* parent): QDialog(parent), pos(size), s1(s1), s2(s2) {
    std::ifstream in("/Users/pavelshykov/Desktop/Qtest/test/land.txt");
    setFixedSize(1000, 1500);
    make_land();
    for (int i = 0; i < size.first; ++i){
        for (int j = 0; j  < size.second; ++j){
            connect(land[pos(i, j)] , SIGNAL(clicked()), this, SLOT(slt()));
        }
    }
}


void Fight::make_land(){
    land.resize(size.first * size.second);
    for (int i = 0; i < size.first * size.second; ++i) {
        std::shared_ptr<squad> grass;
        land[i] = new FightButton(grass, std::pair<int, int>(i % size.first, i / size.first), this);
        land[i]->raise();
        land[i]->show();
    }
    fill();
}

//--------------------------------------------------------------------- paintEvents

void Fight::paintEvent(QPaintEvent *event){
    auto painter = QPainter(this);
    for (int i = 0; i < size.first; ++i){
        for (int j = 0; j  < size.second; ++j){
            auto p = land[pos(i, j)];
            //p->raise();
            p->show();
        }
    }
}


//--------------------------------------------------------------------- pers


void Fight::move_pers(std::shared_ptr<squad>& s, std::pair<int, int> cords){
    if (s->owner == nullptr || s->owner->base->name != uint8_t(-1)){
        auto pnow = std::make_pair(s->x, s->y);
        auto pw = cords;
        land[pos(pnow)]->move(pw);
        land[pos(pw)]->move(pnow);
        std::swap(land[pos(pw)], land[pos(pnow)]);
        land[pos(pnow)]->update();
        land[pos(pw)]->update();
        return;
    }
    auto pnow = std::make_pair(s->x, s->y);
    auto pw = cords;
    if (land[pos(pw)]->obj != nullptr){
        find_near_place(s, pos(pw));
        pw = std::make_pair(s->x, s->y);
        s->attack(*(land[pos(pw)]->obj));
        if (now->count == 0){
            kill(now);
        }
        if (s->count == 0){
            kill(s);
        }
        return;
    }
    land[pos(pnow)]->move(pw);
    land[pos(pw)]->move(pnow);
    std::swap(land[pos(pw)], land[pos(pnow)]);
    land[pos(pnow)]->update();
    land[pos(pw)]->update();
}


void Fight::kill(std::shared_ptr<squad>& s){
    if (s1->count == 0 || (s2->count == 0 && s2->base->name != uint8_t(-1))){
        done(Accepted);
        return;
    }
    std::shared_ptr<squad> tmp;
    auto p = std::make_pair(s->x, s->y);
    auto sum = s->owner;
    sum->killed(s);
    land[pos(p)]->set_squad(tmp);
    if (sum != nullptr && sum->base->name == uint8_t(-1)){
        sum->count = sum->my_squads.size();
        if (sum->count == 0) {
            done(Accepted);
        }
    }
}


//--------------------------------------------------------------------- turn


void Fight::turn(){
    if (queue.empty()){
        for (auto& i : s1->my_squads){
            queue.push(i);
        }
        for (auto& i : s2->my_squads){
            queue.push(i);
        }
        queue.push(std::dynamic_pointer_cast<squad>(s1));
        queue.push(std::dynamic_pointer_cast<squad>(s2));
    }
    now = queue.top();
    queue.pop();
    
    if (now != nullptr && now->count == 0){
        if (now->base->name != uint8_t(-1))
            kill(now);
        turn();
        return;
    }
    
    for (auto i : land){
        i->move_false();
    }
    
    if (now->base->name == uint8_t(-1)){
        turn();
        return;
    }
    
    if (now->owner != nullptr && now->owner->base->name == uint8_t(-1)){
        auto dest = target_search(now);
        move_pers(now, dest);
        turn();
        return;
    }
    
    now->poison_effect();
    can_move({now->x, now->y}, now->base->speed, {size.first + 1, size.second + 1}, true);
}


//--------------------------------------------------------------------- directions


void Fight::find_near_place(std::shared_ptr<squad>& sq, int position){
    std::vector<int> vec = {position - 1,
                            position + 1,
                            position - size.first,
                            position + size.first};
    int sum_pos = pos(sq->x, sq->y);
    auto cmp = [sum_pos](int left, int right) { return abs(left - sum_pos) < abs(right - sum_pos); };
    
    std::sort(vec.begin(), vec.end(), cmp);
    
    for (auto i : vec){
        if (land[i]->obj == nullptr){
            Fight::move_pers(sq, {i % size.first, i / size.first});
            return;
        }
    }
}


bool Fight::can_move(std::pair<int, int> start, int speed, std::pair<int, int> dest, bool ll){
    std::queue<std::pair<int, int>> queue;
    std::vector<bool> used(size.first * size.second, false);
    std::vector<int> len(size.first * size.second, size.first * size.second + 1);
    
    position pos(size);
    len[pos(start)] = 0;
    queue.push(start);
    
    while(!queue.empty()){
        std::pair<int, int> point = queue.front();
        queue.pop();
        int position = pos(point);
        
        if (used[position] == false)
            used[position] = true;
        else continue;
         
        if (len[position] > speed)
            return false;
        
        if (ll){
            land[position]->move_true();
        }
        
        if (point == dest) return true;
        
        std::vector<std::pair<int, int>> neighbor = {  std::make_pair(point.first - 1, point.second),
                                        std::make_pair(point.first + 1, point.second),
                                        std::make_pair(point.first, point.second + 1),
                                        std::make_pair(point.first, point.second - 1) };
        
        for (auto i : neighbor){
            if (is_belong_to_area(i, size) && used[pos(i)] == false && (land[pos(i)]->obj == nullptr || i == dest)){
                queue.push(i);
                len[pos(i)] = len[position] + 1;
            }
        }
    }
    return false;
}


std::pair<int, int> Fight::target_search(shared_ptr& unit){
    std::queue<std::pair<int, int>> queue;
    std::vector<bool> used(size.first * size.second, false);
    std::vector<int> len(size.first * size.second, size.first * size.second + 1);
    std::vector<int> prev(size.first * size.second, 0);
    
    position pos(size);
    auto start = std::make_pair(unit->x, unit->y);
    prev[pos(start)] = 0;
    len[pos(start)] = 0;
    queue.push(start);
    
    while(!queue.empty()){
        auto point = queue.front();
        queue.pop();
        int position = pos(point);
        
        if (used[position] == false)
            used[position] = true;
        else continue;
         
        if (land[position]->obj != nullptr  && land[position]->obj->owner != unit->owner){
            int _prev = position;
            while (_prev != pos(start)){
                if (len[_prev] <= unit->base->speed){
                    return std::make_pair(_prev % size.first, _prev / size.first);
                }
                _prev = prev[_prev];
            }
            return start;
        }
        
        std::vector<std::pair<int, int>> neighbor = {  std::make_pair(point.first - 1, point.second),
                                        std::make_pair(point.first + 1, point.second),
                                        std::make_pair(point.first, point.second + 1),
                                        std::make_pair(point.first, point.second - 1) };
        
        for (auto i : neighbor){
            if (is_belong_to_area(i, size) && used[pos(i)] == false){
                queue.push(i);
                len[pos(i)] = len[position] + 1;
                prev[pos(i)] = position;
            }
        }
    }
    return start;
}


//--------------------------------------------------------------------- interactions


void Fight::slt(){
    auto w = (FightButton*) sender();
    if (started){
        if (w->obj == now){
            QMessageBox::warning(this, "Message", "you skipped your move", 0, 1);
            turn();
            return;
        }
        // for dist squad
        if (w->obj != nullptr){
            if (now->attack(*(w->obj))){
                QMessageBox::warning(this, "Message", "succesfull dist atack", 0, 1);
                if (now->count == 0){
                    kill(now);
                }
                if (w->obj->count == 0){
                    kill(w->obj);
                }
                turn();
                return;
            }
        }
        if (!can_move({now->x, now->y}, now->base->speed, w->cords)){
            QMessageBox::warning(this, "Message", "Your speed is low for this distance", 0, 1);
            raise();
            return;
        }
        if (w->obj == nullptr){
            move_pers(now, w->cords);
            turn();
        }else{
            find_near_place(now, pos(w->cords));
            if (now->attack(*(w->obj))){
                QMessageBox::warning(this, "Message", "succesfull close atack", 0, 1);
                if (now->count == 0){
                    kill(now);
                }
                if (w->obj->count == 0){
                    kill(w->obj);
                }
            }
            turn();
        }
    }else{
        if (land[pos(w->cords)]->obj != nullptr){
            QMessageBox::warning(this, "Message", "this point is busy", 0, 1);
            return;
        }
        if (now->owner == nullptr){
            if (now->base->name == s1->base->name){
                    if (w->cords.first > 1){
                        QMessageBox::warning(this, "Message", "you must stand your units to the FIRST two columns", 0, 1);
                        return;
                    }
            }
            if (now->base->name == s2->base->name){
                    if (w->cords.first < size.first - 2){
                        QMessageBox::warning(this, "Message", "you must stand your units to the LAST two columns", 0, 1);
                        return;
                    }
            }
        }else{
            if (now->owner->base->name == s1->base->name){
                if (w->cords.first > 1){
                    QMessageBox::warning(this, "Message", "you must stand your units to the FIRST two columns", 0, 1);
                    return;
                }
            }
            if (now->owner->base->name == s2->base->name){
                if (w->cords.first < size.first - 2){
                    QMessageBox::warning(this, "Message", "you must stand your units to the LAST two columns", 0, 1);
                    return;
                }
            }
        }
        land[pos(w->cords)]->set_squad(now);
        fill();
    }
}

void Fight::fill(){
    if (co1 < s1->my_squads.size()){
        if (co1 == 0){
            std::string msg = "now " + schools::sc_table.units_names[s1->base->name] + " place his units";
            QMessageBox::warning(this, "Message", msg.c_str(), 0, 1);
        }
        QMessageBox::warning(this, "Message", schools::sc_table.units_names[s1->my_squads[co1]->base->name].c_str(), 0, 1);
        now = s1->my_squads[co1];
        now->moral_balance();
        ++co1;
        return;
    }else if (co1 == s1->my_squads.size()){
        QMessageBox::warning(this, "Message", schools::sc_table.units_names[s1->base->name].c_str(), 0, 1);
        now = std::dynamic_pointer_cast<squad>(s1);
        now->moral_balance();
        ++co1;
        return;
    }else if (s2->base->name == uint8_t(-1)){
        for (auto& i : s2->my_squads){
            i->moral_balance();
            land[pos(i->x, i->y)]->set_squad(i);
        }
    }else if (co2 < s2->my_squads.size()){
        if (co2 == 0){
            std::string msg = "now " + schools::sc_table.units_names[s2->base->name] + " place his units";
            QMessageBox::warning(this, "Message", msg.c_str(), 0, 1);
        }
        QMessageBox::warning(this, "Messege", schools::sc_table.units_names[s2->my_squads[co2]->base->name].c_str(), 0, 1);
        now = s2->my_squads[co2];
        now->moral_balance();
        ++co2;
        return;
    }else if (co2 == s2->my_squads.size()){
        QMessageBox::warning(this, "Messege", schools::sc_table.units_names[s2->base->name].c_str(), 0, 1);
        now = std::dynamic_pointer_cast<squad>(s2);
        now->moral_balance();
        ++co2;
        return;
    }
    //else
    started = true;
    turn();
}
