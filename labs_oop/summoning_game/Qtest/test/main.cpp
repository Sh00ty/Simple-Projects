#include <QApplication>
#include <QMenuBar>
#include "game.hpp"
#include "menu.h"

//schools::school_table schools::sc_table;


int main(int argc, char *argv[]){
//    std::vector<int> prop(100);
//    std::vector<int*> tmp(25);
//    for (int i = 0; i < 25; ++i) tmp[i] = nullptr;
//    Summoner s;
//    s.base = std::make_shared<base_unit>(prop);
//    s.x = 1;
//    s.y = 1;
//    s.base->speed = 5;
//    try{
//        s.can_move<int*>(tmp, {5, 5}, {3, 3});
//    }catch(std::exception& ex){
//        std::cout << ex.what();
//    }
    QApplication a(argc, argv);
    schools::sc_table = schools::school_table(std::ifstream("/Users/pavelshykov/Desktop/labs/summoning_game/units_propertis"));
    schools::sc_table.load_images();
    //auto p = std::dynamic_pointer_cast<Summoner>(Game.lands.summoners[1]);
    //std::cout << p->count;
    start_game_dialog w;
    w.showFullScreen();
    w.setWindowTitle("snummon menu");
    w.show();
        
    return a.exec();
}
