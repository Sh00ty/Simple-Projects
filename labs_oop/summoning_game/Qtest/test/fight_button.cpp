//
//  fight_button.cpp
//  test
//
//  Created by Павел on 08.12.2021.
//

#include "fight_button.hpp"


FightButton::FightButton(std::shared_ptr<squad> obje, std::pair<int, int> cords, QWidget* parent): QPushButton(parent), obj(obje), cords(cords){
    if (obj == nullptr){
        img.load("/Users/pavelshykov/Desktop/Qtest/test/images/grass.png");
        img = img.scaled(size.first, size.second);
    }else{
        img = schools::sc_table.units_images[obj->base->name].scaled(size.first, size.second);
        obj->x = cords.first;
        obj->y = cords.second;
    }
    setFixedSize(size.first, size.second);
    QAbstractButton::move(cords.first * size.first, cords.second * size.second);
    setMouseTracking(false);
    setAttribute(Qt::WA_Hover, true);
}


void FightButton::paintEvent(QPaintEvent *event){
    auto painter = QPainter(this);
    painter.drawImage(0, 0, img.toImage());

    if (isPressed){
        painter.setBrush(QColor(QString("#4000b000")));
        painter.drawRect(QRect(0, 0, size.first, size.second));
    }else if (isHover){
        painter.setBrush(QColor(QString("#40a00040")));
        painter.drawRect(QRect(0, 0, size.first, size.second));
        if (obj != nullptr){
            auto str = QString("hp ") + QString::number(obj->hp);
            str += QString("\ncount ") + QString::number(obj->count);
            str += QString("\naver damage ") + QString::number((obj->base->max_damage + obj->base->min_damage) / 2);
            str += QString("\nmoral ") + QString::number(obj->moral);
            str += QString("\npoisoned long ") + QString::number(obj->poisoned_long);
            setToolTip(str);
        }
    }else if (isMove){
        painter.setBrush(QColor(QString("#40005050")));
        painter.setPen(QPen(QColor("#8100a0d0"), 2));
        painter.drawRect(QRect(0, 0, size.first, size.second));
    }
}


bool FightButton::event(QEvent* event){
    switch(event->type()) {
        case QEvent::HoverEnter:
            isHover = true;
            update();
            return true;
        case QEvent::HoverLeave:
            isHover = false;
            update();
            return true;
        default:
            break;
    }
    return QWidget::event(event);
}


void FightButton::mouseReleaseEvent(QMouseEvent* event){
    isPressed = (event->button() != Qt::LeftButton) && isPressed;
    if (event->button() == Qt::LeftButton)
        emit clicked();
    update();
}


void FightButton::set_squad(std::shared_ptr<squad>& s){
    if (s == nullptr){
        img.load("/Users/pavelshykov/Desktop/Qtest/test/images/grass.png");
        img = img.scaled(size.first, size.second);
        obj = s;
    }else{
        obj = s;
        img = schools::sc_table.units_images[obj->base->name].scaled(size.first, size.second);
        obj->x = cords.first;
        obj->y = cords.second;
    }
    update();
}
