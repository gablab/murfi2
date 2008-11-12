/****************************************************************************
** Meta object code from reading C++ file 'FrMainMenu.h'
**
** Created: Wed 12. Nov 14:32:31 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrMainMenu.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrMainMenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrMainMenu[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_FrMainMenu[] = {
    "FrMainMenu\0"
};

const QMetaObject FrMainMenu::staticMetaObject = {
    { &QMenuBar::staticMetaObject, qt_meta_stringdata_FrMainMenu,
      qt_meta_data_FrMainMenu, 0 }
};

const QMetaObject *FrMainMenu::metaObject() const
{
    return &staticMetaObject;
}

void *FrMainMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrMainMenu))
	return static_cast<void*>(const_cast< FrMainMenu*>(this));
    return QMenuBar::qt_metacast(_clname);
}

int FrMainMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenuBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
