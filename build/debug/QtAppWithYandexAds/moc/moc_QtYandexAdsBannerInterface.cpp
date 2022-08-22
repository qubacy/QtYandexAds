/****************************************************************************
** Meta object code from reading C++ file 'QtYandexAdsBannerInterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../src/QtYandexAds/QtYandexAdsBannerInterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtYandexAdsBannerInterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtYandexAdsBannerInterface_t {
    QByteArrayData data[9];
    char stringdata0[83];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtYandexAdsBannerInterface_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtYandexAdsBannerInterface_t qt_meta_stringdata_QtYandexAdsBannerInterface = {
    {
QT_MOC_LITERAL(0, 0, 26), // "QtYandexAdsBannerInterface"
QT_MOC_LITERAL(1, 27, 6), // "loaded"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 7), // "loading"
QT_MOC_LITERAL(4, 43, 6), // "closed"
QT_MOC_LITERAL(5, 50, 7), // "clicked"
QT_MOC_LITERAL(6, 58, 12), // "errorOccured"
QT_MOC_LITERAL(7, 71, 7), // "AdError"
QT_MOC_LITERAL(8, 79, 3) // "err"

    },
    "QtYandexAdsBannerInterface\0loaded\0\0"
    "loading\0closed\0clicked\0errorOccured\0"
    "AdError\0err"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtYandexAdsBannerInterface[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,
       4,    0,   41,    2, 0x06 /* Public */,
       5,    0,   42,    2, 0x06 /* Public */,
       6,    1,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void QtYandexAdsBannerInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QtYandexAdsBannerInterface *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->loaded(); break;
        case 1: _t->loading(); break;
        case 2: _t->closed(); break;
        case 3: _t->clicked(); break;
        case 4: _t->errorOccured((*reinterpret_cast< AdError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AdError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QtYandexAdsBannerInterface::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QtYandexAdsBannerInterface::loaded)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QtYandexAdsBannerInterface::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QtYandexAdsBannerInterface::loading)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (QtYandexAdsBannerInterface::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QtYandexAdsBannerInterface::closed)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (QtYandexAdsBannerInterface::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QtYandexAdsBannerInterface::clicked)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (QtYandexAdsBannerInterface::*)(AdError );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QtYandexAdsBannerInterface::errorOccured)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QtYandexAdsBannerInterface::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_QtYandexAdsBannerInterface.data,
    qt_meta_data_QtYandexAdsBannerInterface,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QtYandexAdsBannerInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtYandexAdsBannerInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtYandexAdsBannerInterface.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QtYandexAdsBannerInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void QtYandexAdsBannerInterface::loaded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QtYandexAdsBannerInterface::loading()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QtYandexAdsBannerInterface::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QtYandexAdsBannerInterface::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void QtYandexAdsBannerInterface::errorOccured(AdError _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
