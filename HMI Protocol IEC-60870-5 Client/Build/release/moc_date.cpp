/****************************************************************************
** Meta object code from reading C++ file 'date.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../date.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'date.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Date_t {
    QByteArrayData data[14];
    char stringdata0[252];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Date_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Date_t qt_meta_stringdata_Date = {
    {
QT_MOC_LITERAL(0, 0, 4), // "Date"
QT_MOC_LITERAL(1, 5, 34), // "on_calendarWidget_selectionCh..."
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 25), // "on_calendarWidget_clicked"
QT_MOC_LITERAL(4, 67, 4), // "date"
QT_MOC_LITERAL(5, 72, 13), // "on_Ok_clicked"
QT_MOC_LITERAL(6, 86, 17), // "on_Cancel_clicked"
QT_MOC_LITERAL(7, 104, 23), // "on_timeEdit_timeChanged"
QT_MOC_LITERAL(8, 128, 4), // "time"
QT_MOC_LITERAL(9, 133, 29), // "on_timeEdit_2_editingFinished"
QT_MOC_LITERAL(10, 163, 25), // "on_timeEdit_2_timeChanged"
QT_MOC_LITERAL(11, 189, 24), // "on_timeEdit2_timeChanged"
QT_MOC_LITERAL(12, 214, 31), // "on_comboBox_currentIndexChanged"
QT_MOC_LITERAL(13, 246, 5) // "index"

    },
    "Date\0on_calendarWidget_selectionChanged\0"
    "\0on_calendarWidget_clicked\0date\0"
    "on_Ok_clicked\0on_Cancel_clicked\0"
    "on_timeEdit_timeChanged\0time\0"
    "on_timeEdit_2_editingFinished\0"
    "on_timeEdit_2_timeChanged\0"
    "on_timeEdit2_timeChanged\0"
    "on_comboBox_currentIndexChanged\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Date[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    1,   60,    2, 0x08 /* Private */,
       5,    0,   63,    2, 0x08 /* Private */,
       6,    0,   64,    2, 0x08 /* Private */,
       7,    1,   65,    2, 0x08 /* Private */,
       9,    0,   68,    2, 0x08 /* Private */,
      10,    1,   69,    2, 0x08 /* Private */,
      11,    1,   72,    2, 0x08 /* Private */,
      12,    1,   75,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDate,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QTime,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QTime,    8,
    QMetaType::Void, QMetaType::QTime,    8,
    QMetaType::Void, QMetaType::Int,   13,

       0        // eod
};

void Date::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Date *_t = static_cast<Date *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_calendarWidget_selectionChanged(); break;
        case 1: _t->on_calendarWidget_clicked((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 2: _t->on_Ok_clicked(); break;
        case 3: _t->on_Cancel_clicked(); break;
        case 4: _t->on_timeEdit_timeChanged((*reinterpret_cast< const QTime(*)>(_a[1]))); break;
        case 5: _t->on_timeEdit_2_editingFinished(); break;
        case 6: _t->on_timeEdit_2_timeChanged((*reinterpret_cast< const QTime(*)>(_a[1]))); break;
        case 7: _t->on_timeEdit2_timeChanged((*reinterpret_cast< const QTime(*)>(_a[1]))); break;
        case 8: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Date::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Date.data,
      qt_meta_data_Date,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Date::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Date::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Date.stringdata0))
        return static_cast<void*>(const_cast< Date*>(this));
    return QDialog::qt_metacast(_clname);
}

int Date::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
