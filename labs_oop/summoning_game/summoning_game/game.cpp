//
//  game.cpp
//  summoning_game
//
//  Created by Павел on 03.11.2021.
//


#include "game.hpp"
#include "Schools.hpp"
#include <iostream>
#include <QPalette>
#include <QColor>
#include <algorithm>
#include <array>


void find_near_place(shared_ptr sq, int position, std::vector<shared_ptr>& area, point limits){
    std::vector<int> vec = {position - 1,
                            position + 1,
                            position - limits.first,
                            position + limits.first};
    
    auto cmp = [position](int left, int right) { return abs(left - position) < abs(right - position); };
    
    std::sort(vec.begin(), vec.end(), cmp);
    for (auto i : vec){
        if (area[i] == nullptr){
            area[i] = sq;
            sq->x = i % limits.first;
            sq->y = i / limits.first;
            return;
        }
        if (area[i] == sq) return;
    }
}

//---------------------------------------------------------- game


void game::make_names(const std::string& s1, const std::string& s2){
    player1 = s1;
    player2 = s2;
}


void game::start_game(){
    if (game_state == 1) {
        //QMessageBox::warning(this, "Warning", "Game has already been created!!!", 0, 1);
        return;
    }
    //std::ifstream tab("/Users/pavelshykov/Desktop/labs/summoning_game/units_propertis");
    //schools::sc_table = schools::school_table(tab);
    prepare();
    turn();
}


void game::turn(){
    if (queue.empty()){
        for (auto& i : lands.summoners){
            queue.push(std::dynamic_pointer_cast<Summoner>(i));
        }
    }
    now = queue.top();
    queue.pop();
}


void game::prepare(){
    std::vector<std::ifstream> fs;
    fs.emplace_back("/Users/pavelshykov/Desktop/labs/summoning_game/summoners/elph_summoner.txt");
    fs.emplace_back("/Users/pavelshykov/Desktop/labs/summoning_game/summoners/necro_summoner.txt");
    lands = landscape(fs);
    game_state = 1;
    for (auto& i : lands.summoners){
        auto p = i;
        queue.push(std::dynamic_pointer_cast<Summoner>(p));
    }
}


//---------------------------------------------------------- start_game_dialog


start_game_dialog::start_game_dialog(QWidget* parent): QMainWindow(parent) {
    setMenuBar(new Bar(this));
    
    QObject::connect(menuBar(), SIGNAL(sig_start_game()), this, SLOT(start()));
    QObject::connect(menuBar(), SIGNAL(sig_load_game()), this, SLOT(load()));
    QObject::connect(menuBar(), SIGNAL(sig_quit_game()), this, SLOT(quit()));
    
    context_menu = new QMenu(this);
    context_menu->addAction("&Summon", this, SLOT(summon_dialog()));
    context_menu->addSeparator();
    context_menu->addAction("&Accumulate some energy", this, SLOT(accumulation()));
    context_menu->addAction("&Check props", this, SLOT(check_props()));
    connect(context_menu, SIGNAL(triggered(QAction*)), SLOT(slotActivated(QAction*)));
    
    img.load("/Users/pavelshykov/Desktop/Qtest/test/images/zastava.png");
    img = img.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(backgroundRole(), img);
    this->setPalette(palette);
    setCentralWidget(new QWidget);
}


void start_game_dialog::check_props(){
    if (Game.game_state != 1){
        QMessageBox::warning(this, "Warning", "Game isn't created!!!", 0, 1);
        return;
    }
    auto sum = std::dynamic_pointer_cast<object>(Game.now);
    auto stats_d = new Stats(sum);
    stats_d->exec();
    delete stats_d;
}

void start_game_dialog::accumulation(){
    if (Game.game_state != 1){
        QMessageBox::warning(this, "Warning", "Game isn't created!!!", 0, 1);
        return;
    }
    Game.now->energy_accumulation();
    Game.turn();
    auto tmp = dynamic_cast<Warea*>(centralWidget());
    tmp->color_move();
}


void start_game_dialog::summon_dialog(){
    if (Game.game_state != 1){
        QMessageBox::warning(this, "Warning", "Game isn't created!!!", 0, 1);
        return;
    }
    auto dialog = new Summon_dialog(Game.now, this);
    auto res = dialog->run();
    if (res == Summon_dialog::Cant){
        QMessageBox::warning(this, "Message", "You can't summon this unit(your energy is low or you enter some shit)", 0, 1);
    }else if (res == Summon_dialog::Ok){
        Game.turn();
        auto tmp = dynamic_cast<Warea*>(centralWidget());
        tmp->color_move();
    }
    delete dialog;
}


void start_game_dialog::start(){
//        auto* dia = new nameInputDialogs(this);
//        if (dia->exec() == QDialog::Accepted){
//            auto v = dia->name();
//            delete dia;
//            if (v[0].isEmpty() || v[1].isEmpty() || v[0] == v[1]){
//                QMessageBox::warning(this, "Warning", "Your names must be no empty and not the same!!!", 0, 1);
//                return;
//            }
//            Game.make_names(v[0].toStdString(), v[1].toStdString());
//            try{
//                Game.start_game();
//            }catch(...){
//                QMessageBox::warning(this, "nice_exept", "Very nice_Exept", 0, 1);
//            }
//        }
    Game.start_game();
    auto ptr = new Warea(std::make_pair(15, 11), this);
    setCentralWidget(ptr);
    centralWidget()->show();
    QObject::connect(ptr, SIGNAL(closee()), this, SLOT(win_check()));
}
