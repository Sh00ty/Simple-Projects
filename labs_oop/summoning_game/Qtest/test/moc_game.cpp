/****************************************************************************
** Meta object code from reading C++ file 'game.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../labs/summoning_game/summoning_game/game.hpp"
#include <QtGui/qtextcursor.h>
#include <QScreen>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'game.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_start_game_dialog_t {
    const uint offsetsAndSize[24];
    char stringdata0[116];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_start_game_dialog_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_start_game_dialog_t qt_meta_stringdata_start_game_dialog = {
    {
QT_MOC_LITERAL(0, 17), // "start_game_dialog"
QT_MOC_LITERAL(18, 13), // "summon_dialog"
QT_MOC_LITERAL(32, 0), // ""
QT_MOC_LITERAL(33, 12), // "accumulation"
QT_MOC_LITERAL(46, 11), // "check_props"
QT_MOC_LITERAL(58, 5), // "start"
QT_MOC_LITERAL(64, 4), // "load"
QT_MOC_LITERAL(69, 4), // "quit"
QT_MOC_LITERAL(74, 14), // "context_active"
QT_MOC_LITERAL(89, 8), // "QAction*"
QT_MOC_LITERAL(98, 7), // "pAction"
QT_MOC_LITERAL(106, 9) // "win_check"

    },
    "start_game_dialog\0summon_dialog\0\0"
    "accumulation\0check_props\0start\0load\0"
    "quit\0context_active\0QAction*\0pAction\0"
    "win_check"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_start_game_dialog[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x09,    1 /* Protected */,
       3,    0,   63,    2, 0x09,    2 /* Protected */,
       4,    0,   64,    2, 0x09,    3 /* Protected */,
       5,    0,   65,    2, 0x09,    4 /* Protected */,
       6,    0,   66,    2, 0x09,    5 /* Protected */,
       7,    0,   67,    2, 0x09,    6 /* Protected */,
       8,    1,   68,    2, 0x09,    7 /* Protected */,
      11,    0,   71,    2, 0x09,    9 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,

       0        // eod
};

void start_game_dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<start_game_dialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->summon_dialog(); break;
        case 1: _t->accumulation(); break;
        case 2: _t->check_props(); break;
        case 3: _t->start(); break;
        case 4: _t->load(); break;
        case 5: _t->quit(); break;
        case 6: _t->context_active((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 7: _t->win_check(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAction* >(); break;
            }
            break;
        }
    }
}

const QMetaObject start_game_dialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_start_game_dialog.offsetsAndSize,
    qt_meta_data_start_game_dialog,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_start_game_dialog_t
, QtPrivate::TypeAndForceComplete<start_game_dialog, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QAction *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *start_game_dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *start_game_dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_start_game_dialog.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int start_game_dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
