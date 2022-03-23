//
// Created by Grisha on 02.12.2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Button.h" resolved

#include "button.hpp"


Button::Button(QWidget *parent, QColor color) :
        QPushButton(parent), global_coord(Coords{0, 0}),
        mouse_coords(Coords{0, 0}), color(color) {
    //resize(widget_size.height() + 2, widget_size.height() + 2);
    setAttribute(Qt::WA_Hover, true);
    setMouseTracking(true);
    isHover = false;
    isPressed = false;

}

Button::~Button() = default;

