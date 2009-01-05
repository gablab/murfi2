/****************************************************************************
** Meta object code from reading C++ file 'FrToolBar.h'
**
** Created: Sat 27. Dec 00:09:57 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrToolBar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrToolBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrToolBar[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_FrToolBar[] = {
    "FrToolBar\0"
};

const QMetaObject FrToolBar::staticMetaObject = {
    { &QToolBar::staticMetaObject, qt_meta_stringdata_FrToolBar,
      qt_meta_data_FrToolBar, 0 }
};

const QMetaObject *FrToolBar::metaObject() const
{
    return &staticMetaObject;
}

void *FrToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrToolBar))
	return static_cast<void*>(const_cast< FrToolBar*>(this));
    return QToolBar::qt_metacast(_clname);
}

int FrToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
