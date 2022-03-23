//
// Created by Grisha on 02.12.2021.
//

#ifndef QTTEST_BUTTON_H
#define QTTEST_BUTTON_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QPaintEvent>
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QColorDialog>
#include <QScrollBar>

QT_BEGIN_NAMESPACE
namespace Ui { class Button; }
QT_END_NAMESPACE

class Button : public QPushButton {
    Q_OBJECT
    struct Coords{
        int x;
        int y;
    };

public:
    explicit Button(QWidget *parent = nullptr, QColor color = "#ff0000");

    virtual void move(int ax, int ay) {
        global_coord.x = ax;
        global_coord.y = ay;
        QPushButton::move(ax, ay);
    }

    ~Button() override;

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setBrush(QBrush(color));
        //painter.drawEllipse(QRect(1, 1, widget_size.width(), widget_size.height()));
        int state = states::DEFAULT;
        if (isPressed)
            state = states::IS_PRESSED;
        else if (isHover)
            state = states::IS_HOVERED;

        switch (state) {
            case states::DEFAULT:
                return;
            case states::IS_PRESSED:
                painter.setBrush(QBrush(QColor("#4affffff")));
                break;
            case states::IS_HOVERED:
                painter.setBrush(QBrush(QColor("#4a000000")));
                break;
        }
        //painter.drawEllipse(QRect(1, 1, widget_size.width(), widget_size.height()));
    }

    virtual void resize(int w, int h) {
        QWidget::resize(w, h);
    }

    bool event(QEvent* event) override {
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

    void mouseMoveEvent(QMouseEvent* event) override {
        if (isPressed) {
            auto p = event->pos();
            auto dx = mouse_coords.x - p.x();
            auto dy = mouse_coords.y - p.y();
            move(global_coord.x - dx, global_coord.y - dy);
        }
    }

    void mousePressEvent(QMouseEvent* event) override {
        if (!isPressed && event->button() == Qt::LeftButton) {
            isPressed = true;
            auto coords = event->pos();
            mouse_coords.x = coords.x();
            mouse_coords.y = coords.y();
            raise();
            update();
        }
        if (event->button() == Qt::RightButton) {
            if (!bindMenu) {
                bindMenu = new QMenu(this);

                auto action1 = new QAction("createLine", this);
                auto action2 = new QAction("changeColor", this);

                connect(action2, &QAction::triggered, this, &Button::chooseColor);

                bindMenu->addAction(action1);
                bindMenu->addAction(action2);
                bindMenu->addAction("destroyLines");
                connect(bindMenu, &QMenu::aboutToHide, this, &Button::menuHidden);
            }
            bindMenu->move(event->globalPosition().x() , event->globalPosition().y());
            bindMenu->show();
        }
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        isPressed = (event->button() != Qt::LeftButton) && isPressed;
        update();
    }

private slots:
    void menuHidden() {
        isHover = false;
        update();
    }

    void changeColor() {
        color = dialog->currentColor();
        isHover = false;
        update();
    }

    void chooseColor() {
        if (!dialog) {
            dialog = new QColorDialog(this);
            connect(dialog, &QColorDialog::colorSelected, this, &Button::changeColor);
        }
        dialog->show();
        dialog->setVisible(true);
        qDebug() << "show";
    }

    void createScrollBar() {
        if (!scrollBar) {
            scrollBar = new QScrollBar(this);
        }
        scrollBar->move(mouse_coords.x, mouse_coords.y);
        scrollBar->show();
    }



public slots:
    void bindLine() {

    }

private:
    enum states{
        DEFAULT,
        IS_HOVERED,
        IS_PRESSED
    };

    QColorDialog *dialog = nullptr;
    QScrollBar *scrollBar;

    Coords global_coord;
    Coords mouse_coords;
    bool isHover;
    bool isPressed;

    QColor color;
    QMenu *bindMenu = nullptr;
};


#endif //QTTEST_BUTTON_H
