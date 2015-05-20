/****************************************************************************
** Meta object code from reading C++ file 'paintwindow.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "paintwindow.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'paintwindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PaintWindow_t {
    QByteArrayData data[12];
    char stringdata[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_PaintWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_PaintWindow_t qt_meta_stringdata_PaintWindow = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 9),
QT_MOC_LITERAL(2, 22, 0),
QT_MOC_LITERAL(3, 23, 14),
QT_MOC_LITERAL(4, 38, 9),
QT_MOC_LITERAL(5, 48, 8),
QT_MOC_LITERAL(6, 57, 8),
QT_MOC_LITERAL(7, 66, 8),
QT_MOC_LITERAL(8, 75, 14),
QT_MOC_LITERAL(9, 90, 12),
QT_MOC_LITERAL(10, 103, 14),
QT_MOC_LITERAL(11, 118, 13)
    },
    "PaintWindow\0help_line\0\0help_rectangle\0"
    "help_oval\0set_line\0set_rect\0set_oval\0"
    "setColourBlack\0setColourRed\0setColourGreen\0"
    "setColourBlue\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PaintWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08,
       3,    0,   65,    2, 0x08,
       4,    0,   66,    2, 0x08,
       5,    0,   67,    2, 0x08,
       6,    0,   68,    2, 0x08,
       7,    0,   69,    2, 0x08,
       8,    0,   70,    2, 0x08,
       9,    0,   71,    2, 0x08,
      10,    0,   72,    2, 0x08,
      11,    0,   73,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PaintWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PaintWindow *_t = static_cast<PaintWindow *>(_o);
        switch (_id) {
        case 0: _t->help_line(); break;
        case 1: _t->help_rectangle(); break;
        case 2: _t->help_oval(); break;
        case 3: _t->set_line(); break;
        case 4: _t->set_rect(); break;
        case 5: _t->set_oval(); break;
        case 6: _t->setColourBlack(); break;
        case 7: _t->setColourRed(); break;
        case 8: _t->setColourGreen(); break;
        case 9: _t->setColourBlue(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject PaintWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_PaintWindow.data,
      qt_meta_data_PaintWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *PaintWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PaintWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PaintWindow.stringdata))
        return static_cast<void*>(const_cast< PaintWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int PaintWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
