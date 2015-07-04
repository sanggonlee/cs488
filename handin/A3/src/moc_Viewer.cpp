/****************************************************************************
** Meta object code from reading C++ file 'Viewer.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Viewer.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Viewer.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Viewer_t {
    QByteArrayData data[14];
    char stringdata[172];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Viewer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Viewer_t qt_meta_stringdata_Viewer = {
    {
QT_MOC_LITERAL(0, 0, 6),
QT_MOC_LITERAL(1, 7, 13),
QT_MOC_LITERAL(2, 21, 0),
QT_MOC_LITERAL(3, 22, 13),
QT_MOC_LITERAL(4, 36, 16),
QT_MOC_LITERAL(5, 53, 8),
QT_MOC_LITERAL(6, 62, 24),
QT_MOC_LITERAL(7, 87, 11),
QT_MOC_LITERAL(8, 99, 4),
QT_MOC_LITERAL(9, 104, 4),
QT_MOC_LITERAL(10, 109, 11),
QT_MOC_LITERAL(11, 121, 12),
QT_MOC_LITERAL(12, 134, 17),
QT_MOC_LITERAL(13, 152, 18)
    },
    "Viewer\0requestRewalk\0\0resetPosition\0"
    "resetOrientation\0resetAll\0"
    "setToPositionOrientation\0setToJoints\0"
    "undo\0redo\0setToCircle\0setToZBuffer\0"
    "setToBackfaceCull\0setToFrontfaceCull\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Viewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       3,    0,   75,    2, 0x0a,
       4,    0,   76,    2, 0x0a,
       5,    0,   77,    2, 0x0a,
       6,    0,   78,    2, 0x0a,
       7,    0,   79,    2, 0x0a,
       8,    0,   80,    2, 0x0a,
       9,    0,   81,    2, 0x0a,
      10,    0,   82,    2, 0x0a,
      11,    0,   83,    2, 0x0a,
      12,    0,   84,    2, 0x0a,
      13,    0,   85,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,

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
    QMetaType::Void,

       0        // eod
};

void Viewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Viewer *_t = static_cast<Viewer *>(_o);
        switch (_id) {
        case 0: _t->requestRewalk(); break;
        case 1: _t->resetPosition(); break;
        case 2: _t->resetOrientation(); break;
        case 3: _t->resetAll(); break;
        case 4: _t->setToPositionOrientation(); break;
        case 5: _t->setToJoints(); break;
        case 6: _t->undo(); break;
        case 7: _t->redo(); break;
        case 8: _t->setToCircle(); break;
        case 9: _t->setToZBuffer(); break;
        case 10: _t->setToBackfaceCull(); break;
        case 11: _t->setToFrontfaceCull(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Viewer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Viewer::requestRewalk)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Viewer::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_Viewer.data,
      qt_meta_data_Viewer,  qt_static_metacall, 0, 0}
};


const QMetaObject *Viewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Viewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Viewer.stringdata))
        return static_cast<void*>(const_cast< Viewer*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int Viewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Viewer::requestRewalk()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
