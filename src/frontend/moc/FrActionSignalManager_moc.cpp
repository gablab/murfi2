/****************************************************************************
** Meta object code from reading C++ file 'FrActionSignalManager.h'
**
** Created: Tue 7. Oct 20:01:10 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Utils/FrActionSignalManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrActionSignalManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrActionSignalManager[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x0a,
      43,   22,   22,   22, 0x0a,
      62,   22,   22,   22, 0x0a,
      81,   22,   22,   22, 0x0a,
      96,   22,   22,   22, 0x0a,
     116,   22,   22,   22, 0x0a,
     135,   22,   22,   22, 0x0a,
     161,   22,   22,   22, 0x0a,
     186,   22,   22,   22, 0x0a,
     206,   22,   22,   22, 0x0a,
     227,   22,   22,   22, 0x0a,
     247,   22,   22,   22, 0x0a,
     273,   22,   22,   22, 0x0a,
     293,   22,   22,   22, 0x0a,
     311,   22,   22,   22, 0x0a,
     330,   22,   22,   22, 0x0a,
     350,   22,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrActionSignalManager[] = {
    "FrActionSignalManager\0\0OnOpenImageAction()\0"
    "OnSaveTabsAction()\0OnLoadTabsAction()\0"
    "OnExitAction()\0OnSaveToTabAction()\0"
    "OnAddLayerAction()\0OnChangeThresholdAction()\0"
    "OnChangeColormapAction()\0OnViewSliceAction()\0"
    "OnViewMosaicAction()\0OnViewOrthoAction()\0"
    "OnManipulatorToolAction()\0OnVoxelToolAction()\0"
    "OnRoiToolAction()\0OnShowHelpAction()\0"
    "OnShowAboutAction()\0OnTestAction()\0"
};

const QMetaObject FrActionSignalManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FrActionSignalManager,
      qt_meta_data_FrActionSignalManager, 0 }
};

const QMetaObject *FrActionSignalManager::metaObject() const
{
    return &staticMetaObject;
}

void *FrActionSignalManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrActionSignalManager))
	return static_cast<void*>(const_cast< FrActionSignalManager*>(this));
    return QObject::qt_metacast(_clname);
}

int FrActionSignalManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnOpenImageAction(); break;
        case 1: OnSaveTabsAction(); break;
        case 2: OnLoadTabsAction(); break;
        case 3: OnExitAction(); break;
        case 4: OnSaveToTabAction(); break;
        case 5: OnAddLayerAction(); break;
        case 6: OnChangeThresholdAction(); break;
        case 7: OnChangeColormapAction(); break;
        case 8: OnViewSliceAction(); break;
        case 9: OnViewMosaicAction(); break;
        case 10: OnViewOrthoAction(); break;
        case 11: OnManipulatorToolAction(); break;
        case 12: OnVoxelToolAction(); break;
        case 13: OnRoiToolAction(); break;
        case 14: OnShowHelpAction(); break;
        case 15: OnShowAboutAction(); break;
        case 16: OnTestAction(); break;
        }
        _id -= 17;
    }
    return _id;
}