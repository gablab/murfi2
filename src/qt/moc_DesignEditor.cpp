/****************************************************************************
** Meta object code from reading C++ file 'DesignEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DesignEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DesignEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DesignEditor_t {
    QByteArrayData data[9];
    char stringdata[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_DesignEditor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_DesignEditor_t qt_meta_stringdata_DesignEditor = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 15),
QT_MOC_LITERAL(2, 29, 0),
QT_MOC_LITERAL(3, 30, 12),
QT_MOC_LITERAL(4, 43, 15),
QT_MOC_LITERAL(5, 59, 13),
QT_MOC_LITERAL(6, 73, 17),
QT_MOC_LITERAL(7, 91, 6),
QT_MOC_LITERAL(8, 98, 6)
    },
    "DesignEditor\0handleMouseDown\0\0"
    "QMouseEvent*\0handleMouseMove\0handleMouseUp\0"
    "setSelectedColumn\0cancel\0finish\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DesignEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a,
       4,    1,   47,    2, 0x0a,
       5,    1,   50,    2, 0x0a,
       6,    1,   53,    2, 0x0a,
       7,    0,   56,    2, 0x0a,
       8,    0,   57,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DesignEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DesignEditor *_t = static_cast<DesignEditor *>(_o);
        switch (_id) {
        case 0: _t->handleMouseDown((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 1: _t->handleMouseMove((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 2: _t->handleMouseUp((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: _t->setSelectedColumn((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->cancel(); break;
        case 5: _t->finish(); break;
        default: ;
        }
    }
}

const QMetaObject DesignEditor::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DesignEditor.data,
      qt_meta_data_DesignEditor,  qt_static_metacall, 0, 0}
};


const QMetaObject *DesignEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DesignEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DesignEditor.stringdata))
        return static_cast<void*>(const_cast< DesignEditor*>(this));
    return QDialog::qt_metacast(_clname);
}

int DesignEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
