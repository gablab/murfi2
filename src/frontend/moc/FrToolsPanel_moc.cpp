/****************************************************************************
** Meta object code from reading C++ file 'FrToolsPanel.h'
**
** Created: Tue 16. Sep 18:10:43 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrToolsPanel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrToolsPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrToolsPanel[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_FrToolsPanel[] = {
    "FrToolsPanel\0"
};

const QMetaObject FrToolsPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FrToolsPanel,
      qt_meta_data_FrToolsPanel, 0 }
};

const QMetaObject *FrToolsPanel::metaObject() const
{
    return &staticMetaObject;
}

void *FrToolsPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrToolsPanel))
	return static_cast<void*>(const_cast< FrToolsPanel*>(this));
    if (!strcmp(_clname, "Ui::ToolsPanel"))
	return static_cast< Ui::ToolsPanel*>(const_cast< FrToolsPanel*>(this));
    return QWidget::qt_metacast(_clname);
}

int FrToolsPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
