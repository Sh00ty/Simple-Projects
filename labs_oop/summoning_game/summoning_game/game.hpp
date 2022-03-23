//
//  game.hpp
//  summoning_game
//
//  Created by Павел on 03.11.2021.
//

#ifndef game_hpp
#define game_hpp


#include <string>
#include <QPixmap>
#include <QPushButton>
#include "menu.hpp"
#include "landscape.hpp"
#include "Warea.hpp"

using point = std::pair<int, int>;
using queue = std::priority_queue<shared_ptr, std::vector<shared_ptr>, decltype(initiative_cmp)>;


static auto pq_cmp = [] (const std::shared_ptr<Summoner>& left, const std::shared_ptr<Summoner>& right) { return left->base->initiative < right->base->initiative; };

void find_near_place(shared_ptr sq, int position, std::vector<shared_ptr>& area, point limits);

struct game{
    // 0 - isn't created; 1 - is in process; -1 - crashed; 2 - was saved; 3- finished
    int8_t game_state = 0;
    std::string player1;
    std::string player2;
    std::priority_queue<std::shared_ptr<Summoner>, std::vector<std::shared_ptr<Summoner>>, decltype(pq_cmp)> queue = std::priority_queue<std::shared_ptr<Summoner>, std::vector<std::shared_ptr<Summoner>>, decltype(pq_cmp)>(pq_cmp);
    landscape lands;
    
    void prepare();

public:
    std::shared_ptr<Summoner> now;
    game() = default;
    void make_names(const std::string& s1, const std::string& s2);
    void start_game();
    void turn();
};

extern game Game;


//---------------------------------------------------------- start_game_dialog


class start_game_dialog: public QMainWindow{
    Q_OBJECT
    QMenu* context_menu;
    QPixmap img;
protected:
    virtual void contextMenuEvent(QContextMenuEvent *event){
        context_menu->exec(event->globalPos());
    }
    
    virtual void paintEvent(QPaintEvent *event){
        QPainter painter(this);
    }
public:
    start_game_dialog(QWidget* parent = nullptr);
    
protected slots:
    void summon_dialog();
    void accumulation();
    void check_props();
    void start();

    void load(){
        std::cout << "loaded";
    }

    void quit(){
        qApp->quit();
    }
    
    void context_active(QAction* pAction){
        std::cout << pAction->text().toStdString();
    }
    
    void win_check(){
        QPixmap img;
        for (auto& i : Game.lands.summoners){
            auto tmp = std::dynamic_pointer_cast<Summoner>(i);
            if (tmp->count > 0){
                img = schools::sc_table.units_images[tmp->base->name];
                break;
            }
        }
        auto* winner = new QPushButton;
        winner->setFixedSize(1000, 1000);
        img = img.scaled(1000, 1000);
        winner->setIconSize(QSize(1000, 1000));
        winner->setIcon(img);
        winner->raise();
        winner->show();
        centralWidget()->hide();
    }
};


#endif /* game_hpp */
