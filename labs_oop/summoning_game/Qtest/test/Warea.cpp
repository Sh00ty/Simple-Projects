//
//  Warea.cpp
//  test
//
//  Created by Павел on 07.12.2021.
//

#include "game.hpp"
#include "Warea.hpp"
#include "menu.hpp"
#include "Summoner.hpp"


//--------------------------------------------------------------------- constructors


Warea::Warea(std::pair<int, int> size, QWidget* parent): QWidget(parent), size(size), pos(size){
    std::ifstream in("/Users/pavelshykov/Desktop/Qtest/test/land.txt");
    setFixedSize(1000, 1000);
    read_land(in);
    for (int i = 0; i < size.first; ++i){
        for (int j = 0; j  < size.second; ++j){
            connect(land[pos(i, j)] , SIGNAL(clicked()), this, SLOT(slt()));
        }
    }
    color_move();
}


void Warea::read_land(std::ifstream& in){
    land.resize(size.first * size.second);

    for (int i = 0; i < size.first * size.second; ++i) {
        land[i] = new ObjectButton(read_object(in), std::pair<int, int>(i % size.first, i / size.first), this);
        land[i]->raise();
        land[i]->show();
    }
    for (const auto& i : Game.lands.summoners){
        auto p = std::dynamic_pointer_cast<Summoner>(i);
        auto but = new ObjectButton(i, std::make_pair(p->X, p->Y), this);
        delete land[pos(p->X, p->Y)];
        land[pos(p->X, p->Y)] = but;
    }
}


std::shared_ptr<object> Warea::read_object(std::ifstream& in){
    int id;
    std::vector<int> prop;
    prop.resize(30);
    in >> id;
    
    if (id < 0){
        return std::shared_ptr<object>();
    }
    
    if (id < ichest){
        for (int i = 0; i < 11; ++i){
            in >> prop[i];
        }
        auto p = object::Create_object(id);
        auto tmp = std::dynamic_pointer_cast<squad>(p);
        tmp->make_by_short_vec(prop);
        tmp->owner = std::shared_ptr<Summoner>();
        return p;
    }
    auto che = object::Create_object(id);
    auto tmp = std::dynamic_pointer_cast<chest>(che);
    for (int i = 0; i < 4; ++i){
        in >> prop[i];
    }
    tmp->make_by_vec(prop);
    return che;
}


//--------------------------------------------------------------------- paintEvents


void Warea::paintEvent(QPaintEvent *event){
    auto painter = QPainter(this);
    for (int i = 0; i < size.first; ++i){
        for (int j = 0; j  < size.second; ++j){
            auto p = land[pos(i, j)];
            //p->raise();
            p->show();
        }
    }
}


void Warea::move_pers(std::shared_ptr<Summoner>& s, std::pair<int, int> cords){
    auto pnow = std::make_pair(s->X, s->Y);
    auto pw = cords;
    land[pos(pnow)]->move(pw);
    land[pos(pw)]->move(pnow);
    std::swap(land[pos(pw)], land[pos(pnow)]);
    land[pos(pnow)]->update();
    land[pos(pw)]->update();
}


void Warea::slt(){
    auto w = (ObjectButton*) sender();
    if (!can_move({Game.now->X, Game.now->Y}, Game.now->base->speed, w->cords)){
        QMessageBox::warning(this, "Messege", "Your speed is low for this distance", 0, 1);
        update();
        return;
    }
    if (w->obj == nullptr){
        move_pers(Game.now, w->cords);
        Game.turn();
        color_move();
    }else{
        if (w->obj->X == Game.now->X && w->obj->Y == Game.now->Y) return;
        find_near_place(Game.now, pos(w->cords));
        auto stats_d = new Stats(w->obj);
        stats_d->exec();
        update();
        if (w->obj->interaction(Game.now) == efight){
            for(auto& i: Game.lands.summoners){
                auto tmp = std::dynamic_pointer_cast<Summoner>(i);
                if (tmp->count == 0){
                    close();
                    emit closee();
                    return;
                }
            }
            auto sq = std::dynamic_pointer_cast<squad>(w->obj);
            if (sq != nullptr && sq->count == 0) w->kill();
        }
        Game.turn();
        color_move();
    }
}


//--------------------------------------------------------------------- turns


void Warea::color_move(){
    for (auto i : land){
        i->move_false();
    }
    can_move({Game.now->X, Game.now->Y}, Game.now->base->speed, {size.first + 1, size.second + 1}, true);
    QMessageBox::warning(this, "Message", schools::sc_table.units_names[Game.now->base->name].c_str(), 0, 1);
}


//--------------------------------------------------------------------- directions


bool Warea::can_move(std::pair<int, int> start, int speed, std::pair<int, int> dest, bool ll){
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


void Warea::find_near_place(std::shared_ptr<Summoner> sq, int position){
    std::vector<int> vec = {position - 1,
                            position + 1,
                            position - size.first,
                            position + size.first};
    int sum_pos = pos(sq->X, sq->Y);
    auto cmp = [sum_pos](int left, int right) { return abs(left - sum_pos) < abs(right - sum_pos); };
    
    std::sort(vec.begin(), vec.end(), cmp);
    
    for (auto i : vec){
        if (land[i]->obj == nullptr){
            move_pers(sq, {i % size.first, i / size.first});
            return;
        }
    }
}

