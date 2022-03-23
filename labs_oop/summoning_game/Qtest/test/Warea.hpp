//
//  Warea.hpp
//  test
//
//  Created by Павел on 07.12.2021.
//

#ifndef Warea_hpp
#define Warea_hpp

#include "MyPushButton.hpp"
#include <QWidget>
#include <memory>
#include <iostream>
#include <queue>
#include <vector>


class Warea: public QWidget{
    Q_OBJECT;
    
    std::vector<ObjectButton*> land;
    std::pair<int, int> size;
    position pos;
    
public:
    
    Warea(std::pair<int, int> size, QWidget* parent = nullptr);
    
    void read_land(std::ifstream& in);
    
    std::shared_ptr<object> read_object(std::ifstream& in);
    
    
    bool can_move(std::pair<int, int> start, int speed, std::pair<int, int> dest, bool ll = false);
    
    void move_pers(std::shared_ptr<Summoner>& s, std::pair<int, int> cords);
    
    void find_near_place(std::shared_ptr<Summoner> sq, int position);
    
    void color_move();
    
protected:
    void paintEvent(QPaintEvent *event) override;
    
    
public slots:
    void slt();

    
signals:
    void closee();
    
};




#endif /* Warea_hpp */
