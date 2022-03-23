//
//  fight_button.hpp
//  test
//
//  Created by Павел on 08.12.2021.
//

#ifndef fight_button_hpp
#define fight_button_hpp

#include <QPushButton>
#include <QPixmap>
#include <QApplication>
#include <QWidget>
#include <QObject>
#include "Summoner.hpp"

class FightButton: public QPushButton{
    Q_OBJECT;
    
    std::pair<int, int> size = {78, 88};
    QPixmap img;
    bool isHover = false;
    bool isPressed = false;
    bool isMove = false;
    
public:
    std::shared_ptr<squad> obj;
    std::pair<int, int> cords;
    
    FightButton(std::shared_ptr<squad> obje, std::pair<int, int> cords, QWidget* parent = nullptr);
    
    void move(std::pair<int, int> point){
        cords.first = point.first;
        cords.second = point.second;
        if (obj != nullptr){
            obj->x = point.first;
            obj->y = point.second;
        }
        QAbstractButton::move(cords.first * size.first, cords.second * size.second);
    }
    
    void set_squad(std::shared_ptr<squad>& s);
    
    void move_true(){
        isMove = true;
        update();
    }
    
    
    void move_false(){
        isMove = false;
        update();
    }
    
signals:
    void clicked();
    
protected:
    
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    
    void mousePressEvent(QMouseEvent* event) override{
        if (!isPressed && event->button() == Qt::LeftButton) {
            isPressed = true;
            raise();
            update();
        }
    }
    
    
};



#endif /* fight_button_hpp */
