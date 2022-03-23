//
//  MyPushButton.cpp
//  test
//
//  Created by Павел on 07.12.2021.
//

#include "MyPushButton.hpp"


ObjectButton::ObjectButton(std::shared_ptr<object> obje, std::pair<size_t, size_t> cords, QWidget* parent): QPushButton(parent), obj(obje), cords(cords){
    if (obj == nullptr){
        img.load("/Users/pavelshykov/Desktop/Qtest/test/images/grass.png");
        img = img.scaled(size.first, size.second);
    }else if (std::dynamic_pointer_cast<chest>(obj) != nullptr){
        img.load("/Users/pavelshykov/Desktop/Qtest/test/images/giant.png");
        img = img.scaled(size.first, size.second);
        obj->X = cords.first;
        obj->Y = cords.second;
    }else{
        auto tmp = std::dynamic_pointer_cast<squad>(obj);
        img = schools::sc_table.units_images[tmp->base->name].scaled(size.first, size.second);
        obj->X = cords.first;
        obj->Y = cords.second;
    }
    setFixedSize(size.first, size.second);
    QAbstractButton::move(cords.first * size.first, cords.second * size.second);
    setMouseTracking(false);
    setAttribute(Qt::WA_Hover, true);
    
}


void ObjectButton::paintEvent(QPaintEvent *event){
    auto painter = QPainter(this);
    painter.drawImage(0, 0, img.toImage());

    if (isPressed){
        painter.setBrush(QColor(QString("#4000b000")));
        painter.drawRect(QRect(0, 0, size.first, size.second));
    }else if(isHover){
        painter.setBrush(QColor(QString("#40a00040")));
        painter.drawRect(QRect(0, 0, size.first, size.second));
    }else if (isMove){
        painter.setBrush(QColor(QString("#40005050")));
        painter.setPen(QPen(QColor("#8100a0d0"), 2));
        painter.drawRect(QRect(0, 0, size.first, size.second));
    }
}

bool ObjectButton::event(QEvent* event){
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

void ObjectButton::mouseReleaseEvent(QMouseEvent* event){
    isPressed = (event->button() != Qt::LeftButton) && isPressed;
    if (event->button() == Qt::LeftButton)
        emit clicked();
    update();
}
