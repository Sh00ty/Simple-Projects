//
//  MyPushButton.hpp
//  test
//
//  Created by Павел on 07.12.2021.
//

#ifndef MyPushButton_hpp
#define MyPushButton_hpp

#include <QPushButton>
#include <QPixmap>
#include <QApplication>
#include <QWidget>
#include <QObject>
#include "Summoner.hpp"

class ObjectButton: public QPushButton{
    Q_OBJECT;
    
    std::pair<int, int> size = {78, 88};
    QPixmap img;
    bool isHover = false;
    bool isPressed = false;
    bool isMove = true;
public:
    std::shared_ptr<object> obj;
    std::pair<int, int> cords;
    // make & at shared_ptr
    ObjectButton(std::shared_ptr<object> obje, std::pair<size_t, size_t> cords, QWidget* parent = nullptr);
    
    void move(std::pair<int, int> point){
        cords.first = point.first;
        cords.second = point.second;
        if (obj != nullptr){
            obj->X = point.first;
            obj->Y = point.second;
        }
        QAbstractButton::move(cords.first * size.first, cords.second * size.second);
    }
    
    void move_true(){
        isMove = true;
        update();
    }
    
    
    void move_false(){
        isMove = false;
        update();
    }
    
    void kill(){
        img.load("/Users/pavelshykov/Desktop/Qtest/test/images/grass.png");
        img = img.scaled(size.first, size.second);
        obj = std::shared_ptr<object>();
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
        if (!isPressed && event->button() == Qt::RightButton) {
            isPressed = true;
            raise();
            update();
        }
    }
    
};


#endif /* MyPushButton_hpp */
