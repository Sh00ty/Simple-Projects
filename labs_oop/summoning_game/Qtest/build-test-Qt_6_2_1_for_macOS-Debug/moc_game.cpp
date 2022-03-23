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
    const uint offsetsAndSize[22];
    char stringdata0[106];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_start_game_dialog_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_start_game_dialog_t qt_meta_stringdata_start_game_dialog = {
    {
QT_MOC_LITERAL(0, 17), // "start_game_dialog"
QT_MOC_LITERAL(18, 14), // "context_active"
QT_MOC_LITERAL(33, 0), // ""
QT_MOC_LITERAL(34, 8), // "QAction*"
QT_MOC_LITERAL(43, 7), // "pAction"
QT_MOC_LITERAL(51, 13), // "summon_dialog"
QT_MOC_LITERAL(65, 12), // "accumulation"
QT_MOC_LITERAL(78, 11), // "check_props"
QT_MOC_LITERAL(90, 5), // "start"
QT_MOC_LITERAL(96, 4), // "load"
QT_MOC_LITERAL(101, 4) // "quit"

    },
    "start_game_dialog\0context_active\0\0"
    "QAction*\0pAction\0summon_dialog\0"
    "accumulation\0check_props\0start\0load\0"
    "quit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_start_game_dialog[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x0a,    1 /* Public */,
       5,    0,   59,    2, 0x0a,    3 /* Public */,
       6,    0,   60,    2, 0x0a,    4 /* Public */,
       7,    0,   61,    2, 0x0a,    5 /* Public */,
       8,    0,   62,    2, 0x0a,    6 /* Public */,
       9,    0,   63,    2, 0x0a,    7 /* Public */,
      10,    0,   64,    2, 0x0a,    8 /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void start_game_dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<start_game_dialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->context_active((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 1: _t->summon_dialog(); break;
        case 2: _t->accumulation(); break;
        case 3: _t->check_props(); break;
        case 4: _t->start(); break;
        case 5: _t->load(); break;
        case 6: _t->quit(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
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
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QAction *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


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
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
