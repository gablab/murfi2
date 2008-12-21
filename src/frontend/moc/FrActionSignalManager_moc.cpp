/****************************************************************************
** Meta object code from reading C++ file 'FrActionSignalManager.h'
**
** Created: Sun 21. Dec 00:35:48 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Utils/FrActionSignalManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrActionSignalManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrActionSignalManager[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x0a,
      43,   22,   22,   22, 0x0a,
      61,   22,   22,   22, 0x0a,
      80,   22,   22,   22, 0x0a,
      99,   22,   22,   22, 0x0a,
     114,   22,   22,   22, 0x0a,
     134,   22,   22,   22, 0x0a,
     153,   22,   22,   22, 0x0a,
     175,   22,   22,   22, 0x0a,
     197,   22,   22,   22, 0x0a,
     214,   22,   22,   22, 0x0a,
     234,   22,   22,   22, 0x0a,
     255,   22,   22,   22, 0x0a,
     275,   22,   22,   22, 0x0a,
     296,   22,   22,   22, 0x0a,
     322,   22,   22,   22, 0x0a,
     342,   22,   22,   22, 0x0a,
     360,   22,   22,   22, 0x0a,
     379,   22,   22,   22, 0x0a,
     399,   22,   22,   22, 0x0a,
     414,   22,   22,   22, 0x0a,
     437,   22,   22,   22, 0x0a,
     462,   22,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrActionSignalManager[] = {
    "FrActionSignalManager\0\0OnOpenImageAction()\0"
    "OnOpenDataStore()\0OnSaveTabsAction()\0"
    "OnLoadTabsAction()\0OnExitAction()\0"
    "OnSaveToTabAction()\0OnNewLayerAction()\0"
    "OnDeleteLayerAction()\0OnChangeLayerAction()\0"
    "OnNewROIAction()\0OnViewSliceAction()\0"
    "OnViewMosaicAction()\0OnViewOrthoAction()\0"
    "OnResetImageAction()\0OnManipulatorToolAction()\0"
    "OnVoxelToolAction()\0OnRoiToolAction()\0"
    "OnShowHelpAction()\0OnShowAboutAction()\0"
    "OnTestAction()\0OnLayerParamsChanged()\0"
    "OnLayerColormapChanged()\0OnRoiToolChanged()\0"
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
        case 1: OnOpenDataStore(); break;
        case 2: OnSaveTabsAction(); break;
        case 3: OnLoadTabsAction(); break;
        case 4: OnExitAction(); break;
        case 5: OnSaveToTabAction(); break;
        case 6: OnNewLayerAction(); break;
        case 7: OnDeleteLayerAction(); break;
        case 8: OnChangeLayerAction(); break;
        case 9: OnNewROIAction(); break;
        case 10: OnViewSliceAction(); break;
        case 11: OnViewMosaicAction(); break;
        case 12: OnViewOrthoAction(); break;
        case 13: OnResetImageAction(); break;
        case 14: OnManipulatorToolAction(); break;
        case 15: OnVoxelToolAction(); break;
        case 16: OnRoiToolAction(); break;
        case 17: OnShowHelpAction(); break;
        case 18: OnShowAboutAction(); break;
        case 19: OnTestAction(); break;
        case 20: OnLayerParamsChanged(); break;
        case 21: OnLayerColormapChanged(); break;
        case 22: OnRoiToolChanged(); break;
        }
        _id -= 23;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
