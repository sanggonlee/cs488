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
    QByteArrayData data[12];
    char stringdata[161];
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
QT_MOC_LITERAL(3, 22, 3),
QT_MOC_LITERAL(4, 26, 10),
QT_MOC_LITERAL(5, 37, 15),
QT_MOC_LITERAL(6, 53, 18),
QT_MOC_LITERAL(7, 72, 20),
QT_MOC_LITERAL(8, 93, 16),
QT_MOC_LITERAL(9, 110, 19),
QT_MOC_LITERAL(10, 130, 15),
QT_MOC_LITERAL(11, 146, 13)
    },
    "Viewer\0statusChanged\0\0msg\0reset_view\0"
    "setToViewRotate\0setToViewTranslate\0"
    "setToViewPerspective\0setToModelRotate\0"
    "setToModelTranslate\0setToModelScale\0"
    "setToViewport\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Viewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       4,    0,   62,    2, 0x0a,
       5,    0,   63,    2, 0x0a,
       6,    0,   64,    2, 0x0a,
       7,    0,   65,    2, 0x0a,
       8,    0,   66,    2, 0x0a,
       9,    0,   67,    2, 0x0a,
      10,    0,   68,    2, 0x0a,
      11,    0,   69,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
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
        case 0: _t->statusChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->reset_view(); break;
        case 2: _t->setToViewRotate(); break;
        case 3: _t->setToViewTranslate(); break;
        case 4: _t->setToViewPerspective(); break;
        case 5: _t->setToModelRotate(); break;
        case 6: _t->setToModelTranslate(); break;
        case 7: _t->setToModelScale(); break;
        case 8: _t->setToViewport(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Viewer::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Viewer::statusChanged)) {
                *result = 0;
            }
        }
    }
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

// SIGNAL 0
void Viewer::statusChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
