/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN10MainWindowE = QtMocHelpers::stringData(
    "MainWindow",
    "on_pushButton_Read_clicked",
    "",
    "on_pushButton_Create_clicked",
    "on_pushButton_Update_clicked",
    "on_pushButton_Delete_clicked",
    "on_tableView_clicked",
    "QModelIndex",
    "index",
    "sortDateAscending",
    "sortDateDescending",
    "onSortIndicatorChanged",
    "logicalIndex",
    "Qt::SortOrder",
    "order",
    "exportToPdf",
    "calculateAttendanceStats",
    "showAttendanceStatsDialog",
    "exportStatsTableToPdf",
    "QTableWidget*",
    "table",
    "on_pushButton_MatchStats_clicked",
    "filterByTypeMatch",
    "on_pushButton_Calendar_clicked",
    "onCalendarClicked",
    "date",
    "on_pushButton_Simulate_clicked",
    "updateSimulation"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN10MainWindowE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  122,    2, 0x08,    1 /* Private */,
       3,    0,  123,    2, 0x08,    2 /* Private */,
       4,    0,  124,    2, 0x08,    3 /* Private */,
       5,    0,  125,    2, 0x08,    4 /* Private */,
       6,    1,  126,    2, 0x08,    5 /* Private */,
       9,    0,  129,    2, 0x08,    7 /* Private */,
      10,    0,  130,    2, 0x08,    8 /* Private */,
      11,    2,  131,    2, 0x08,    9 /* Private */,
      15,    0,  136,    2, 0x08,   12 /* Private */,
      16,    0,  137,    2, 0x08,   13 /* Private */,
      17,    0,  138,    2, 0x08,   14 /* Private */,
      18,    1,  139,    2, 0x08,   15 /* Private */,
      21,    0,  142,    2, 0x08,   17 /* Private */,
      22,    0,  143,    2, 0x08,   18 /* Private */,
      23,    0,  144,    2, 0x08,   19 /* Private */,
      24,    1,  145,    2, 0x08,   20 /* Private */,
      26,    0,  148,    2, 0x08,   22 /* Private */,
      27,    0,  149,    2, 0x08,   23 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 13,   12,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDate,   25,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ZN10MainWindowE.offsetsAndSizes,
    qt_meta_data_ZN10MainWindowE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN10MainWindowE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'on_pushButton_Read_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_Create_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_Update_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_Delete_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_tableView_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'sortDateAscending'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sortDateDescending'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSortIndicatorChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<Qt::SortOrder, std::false_type>,
        // method 'exportToPdf'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'calculateAttendanceStats'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showAttendanceStatsDialog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'exportStatsTableToPdf'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTableWidget *, std::false_type>,
        // method 'on_pushButton_MatchStats_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'filterByTypeMatch'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_Calendar_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCalendarClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDate &, std::false_type>,
        // method 'on_pushButton_Simulate_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateSimulation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_pushButton_Read_clicked(); break;
        case 1: _t->on_pushButton_Create_clicked(); break;
        case 2: _t->on_pushButton_Update_clicked(); break;
        case 3: _t->on_pushButton_Delete_clicked(); break;
        case 4: _t->on_tableView_clicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 5: _t->sortDateAscending(); break;
        case 6: _t->sortDateDescending(); break;
        case 7: _t->onSortIndicatorChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Qt::SortOrder>>(_a[2]))); break;
        case 8: _t->exportToPdf(); break;
        case 9: _t->calculateAttendanceStats(); break;
        case 10: _t->showAttendanceStatsDialog(); break;
        case 11: _t->exportStatsTableToPdf((*reinterpret_cast< std::add_pointer_t<QTableWidget*>>(_a[1]))); break;
        case 12: _t->on_pushButton_MatchStats_clicked(); break;
        case 13: _t->filterByTypeMatch(); break;
        case 14: _t->on_pushButton_Calendar_clicked(); break;
        case 15: _t->onCalendarClicked((*reinterpret_cast< std::add_pointer_t<QDate>>(_a[1]))); break;
        case 16: _t->on_pushButton_Simulate_clicked(); break;
        case 17: _t->updateSimulation(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QTableWidget* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN10MainWindowE.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}
QT_WARNING_POP
