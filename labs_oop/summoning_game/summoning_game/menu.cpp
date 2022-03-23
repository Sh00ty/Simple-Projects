//
//  menu.cpp
//  summoning_game
//
//  Created by Павел on 18.11.2021.
//

#include "menu.hpp"
#include "game.hpp"
game Game;

using namespace schools;


//---------------------------------------------------------------------  Bar


Bar::Bar(QWidget* parent): QMenuBar(parent){

    QMenu* main_menu = addMenu(tr("&settings"));

    main_menu->addAction("&new game", this, SLOT(start_game()));
    main_menu->addAction("&load game", this, SLOT(load_game()));
    main_menu->addSeparator();
    main_menu->addAction("&bquit", this, SLOT(quit_game()), Qt::CTRL | Qt::Key_Q);
    
    QMenu* main_menu2 = addMenu(tr("&sec2"));
    main_menu2->addAction("&example1");
    main_menu2->addAction("&example2");
    main_menu2->addSeparator();
    main_menu2->addAction("&example3");
}


//---------------------------------------------------------------------  Summon_dialog


std::pair<uint8_t, uint8_t> sumon_menu(){
    uint8_t sc;
    uint8_t name;
    auto it = sc_table.map.end();
    while(true){
        int c = 0;
        for (auto& i : sc_table.schools_names){
            std::cout << "\n" << c << "  "<< i;
            ++c;
        }
        std::cout << "\nchoose number of school::";
        sc = MLT::get_num<uint8_t>();
        it = sc_table.map.find(sc);
        if (it == sc_table.map.end()){
            std::cout << "\nThere is no such school, enter it again";
            continue;
        }
        break;
    }
    while(true){
        int first = it->second.begin()->bu->name;
        int last = -1;
        
        for (auto& i : it->second){
            std::cout << "\n" << last << "  " << sc_table.units_names[i.bu->name];
            ++last;
        }
        std::cout << "\nchoose number of unit::";
        name = MLT::get_num<uint8_t>();
        if (name < first || name > last){
            std::cout << "\nThis unit not from school you have choosen or it doesn't exists";
            continue;
        }
        break;
    }
    return std::make_pair(sc, name);
}


//---------------------------------------------------------------------  nameInputDialogs


nameInputDialogs::nameInputDialogs(QWidget* parent): QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint){

    _name1 = new QLineEdit(this);
    _name2 = new QLineEdit(this);

    QLabel* label1 = new QLabel("&Enter name for first pers ");
    QLabel* label2 = new QLabel("&Enter name for second pers ");

    label1->setBuddy(_name1);
    label2->setBuddy(_name2);

    QPushButton* ok_button = new QPushButton("&Ok");
    QPushButton* cancel_button = new QPushButton("&Cancel");

    connect(ok_button, SIGNAL(clicked()), SLOT(accept()));
    connect(cancel_button, SIGNAL(clicked()), SLOT(reject()));

    auto* layout = new QGridLayout;
    layout->addWidget(label1, 0, 0);
    layout->addWidget(_name1, 0, 1);
    layout->addWidget(label2, 1, 0);
    layout->addWidget(_name2, 1, 1);
    layout->addWidget(ok_button, 2, 1);
    layout->addWidget(cancel_button, 2, 2);
    setLayout(layout);
}


std::vector<QString> nameInputDialogs::name() const{
    std::vector<QString> v;
    v.push_back(_name1->text());
    v.push_back(_name2->text());
    return v;
}

//---------------------------------------------------------------------  Summon_dialog

Summon_dialog::Summon_dialog(std::shared_ptr<Summoner>& summoner, QWidget* parent): QWidget(parent), summoner(summoner) {
    dialog = new school_choice_dialog(summoner, this);
    setWindowTitle("Summon menu");
    dialog->resize(500, 700);
}


Summon_dialog::~Summon_dialog(){
    delete dialog;
}


Summon_dialog::Choise Summon_dialog::run(){
    auto res = dialog->exec();
    if (res == QDialog::Rejected) return Canceled;
    if (dialog->res == true) return Ok;
    return Cant;
}


//---------------------------------------------------------------------  MyButton

MyButton::MyButton(school_choice_dialog* parent, int n): QPushButton(parent), n(n), parent(parent) {
    connect(this, SIGNAL(clicked()), this, SLOT(signall()));
}


void MyButton::signall(){
    parent->fill(n);
}

//--------------------------------------------------------------------- school_choice_dialog

school_choice_dialog::school_choice_dialog(std::shared_ptr<Summoner>& ptr, QWidget* parent):
        QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), summoner(ptr){
    using namespace schools;
    QVBoxLayout* layout = new QVBoxLayout;
    for (size_t i = 0; i < sc_table.school_count(); ++i){
        auto mb = new MyButton(this, i);
        mb->setText(sc_table.schools_names[i].c_str());
        mb->setGeometry(QRect(0, 0, 20, 33));
        layout->addWidget(mb);
    }
    
    QPushButton* cancel_button = new QPushButton("&Cancel");
    connect(cancel_button, SIGNAL(clicked()), SLOT(reject()));
    layout->addWidget(cancel_button);
    
    setLayout(layout);
}

void school_choice_dialog::summon_accept(){
    int count = name->text().toInt();
    if (count > 0 && count <= _max_count){
        summoner->summon(school_choice, unit_name, count);
        res = true;
    }
    done(Accepted);
}


void school_choice_dialog::fill(int n){
    if (!what){
        school_choice = n;
        ++what;
        
        delete this->layout();
        qDeleteAll(this->children());
        
        QVBoxLayout* layout2 = new QVBoxLayout;
        for (auto& i: sc_table.map[school_choice]){
            if (i.min_knowledge > summoner->knowledge[school_choice]) continue;
            auto mb = new MyButton(this, i.bu->name);
            mb->setText(sc_table.units_names[i.bu->name].c_str());
            layout2->addWidget(mb);
        }

        QPushButton* cancel_button = new QPushButton("&Cancel");
        connect(cancel_button, SIGNAL(clicked()), SLOT(reject()));

        layout2->addWidget(cancel_button);
        setLayout(layout2);
    }else {
        unit_name = n;
        Summon();

    }
}


void school_choice_dialog::Summon(){
    
    delete this->layout();
    qDeleteAll(this->children());
    
   
    auto it = schools::sc_table.get(school_choice, unit_name);
    //int max_count = summoner->energy / (it->exp_per_unit_to_summon * it->lev_hed_rat);
    int max_count = summoner->energy / (it->exp_per_unit_to_summon);
    if (max_count == 0) done(Accepted);
    _max_count = max_count;
    
    QString str;
    str.setNum(max_count, 10);
    
    name = new QLineEdit(this);
    
    auto lab = new QLabel("&You can summon less then " + str + ":");

    lab->setBuddy(name);

    QPushButton* ok_button = new QPushButton("&Ok");
    QPushButton* cancel_button = new QPushButton("&Cancel");

    connect(ok_button, SIGNAL(clicked()), this, SLOT(summon_accept()));
    connect(cancel_button, SIGNAL(clicked()), SLOT(reject()));

    auto lay = new QGridLayout(this);
    lay->addWidget(lab, 0, 0);
    lay->addWidget(name, 0, 1);

    lay->addWidget(ok_button, 1, 0);
    lay->addWidget(cancel_button, 1, 1);

    setLayout(lay);
}
