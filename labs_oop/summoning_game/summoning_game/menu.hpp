//
//  menu.hpp
//  summoning_game
//
//  Created by Павел on 18.11.2021.
//

#ifndef menu_hpp
#define menu_hpp

#include "Summoner.hpp"
#include <vector>
#include <QAction>
#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QDialog>
#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QtGui>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>


std::pair<int, int> move_menu(const std::pair<int, int> limits);
std::pair<uint8_t, uint8_t> sumon_menu();
void turn_menu();
std::pair<int, int> target_search(const std::vector<shared_ptr> area, std::pair<int, int> lim, shared_ptr& unit);
void print_summoner_characters(std::shared_ptr<Summoner>& s);


class school_choice_dialog;


//--------------------------------------------------------------------- move_dialog



//--------------------------------------------------------------------- menus

class Bar: public QMenuBar{
    Q_OBJECT

public:
    Bar(QWidget* parent = nullptr);
public slots:
    void start_game(){
        std::cout << "\nstart";
        emit sig_start_game();
    }

    void load_game(){
        emit sig_load_game();
    }

    void quit_game(){
        emit sig_quit_game();
    }

signals:
    void sig_start_game();
    void sig_load_game();
    void sig_quit_game();
};

//---------------------------------------------------------------------  nameInputDialogs

class nameInputDialogs: public QDialog{
    Q_OBJECT

    QLineEdit* _name1;
    QLineEdit* _name2;

public:
    nameInputDialogs(QWidget* parent = nullptr);
    std::vector<QString> name() const;
};

//---------------------------------------------------------------------  Summon_dialog

class Summon_dialog: public QWidget{
    Q_OBJECT
    
    std::shared_ptr<Summoner> summoner;
    school_choice_dialog* dialog;
public:
    
    enum Choise{Canceled = 0, Cant, Ok};
    
    Summon_dialog(std::shared_ptr<Summoner>& summoner, QWidget* parent = nullptr);
    
    Choise run();
    
    ~Summon_dialog();
};

//---------------------------------------------------------------------  MyButton

class MyButton: public QPushButton{
    Q_OBJECT
    int n;
    school_choice_dialog* parent;
public:
    MyButton(school_choice_dialog* parent, int n);

public slots:
    void signall();
};

//--------------------------------------------------------------------- school_choice_dialog

class school_choice_dialog: public QDialog{
    Q_OBJECT
    int what = 0;
    std::shared_ptr<Summoner> summoner;
    QLineEdit* name = nullptr;
    int school_choice;
    int unit_name;
    int _max_count;
public:
    bool res = false;

    school_choice_dialog(std::shared_ptr<Summoner>& ptr, QWidget* parent = nullptr);

    ~school_choice_dialog() {if (name) delete name;}
    
    void fill(int n);

    void Summon();
    
public slots:
    void summon_accept();
};


//--------------------------------------------------------------------- print_stats



class Stats: public QDialog{
    Q_OBJECT
    std::shared_ptr<object> obj;
    
public:
    Stats(std::shared_ptr<object>& ob, QWidget* parent = nullptr): QDialog(parent), obj(ob){
        setLayout(obj->print_stats());
        QPushButton* cancel_button = new QPushButton("&Cancel");
        connect(cancel_button, SIGNAL(clicked()), SLOT(reject()));
        layout()->addWidget(cancel_button);
    }
};






#endif /* menu_hpp */
