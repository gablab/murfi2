/****************************************************************************
** Meta object code from reading C++ file 'FrActionSignalManager.h'
**
** Created: Tue 27. Jan 17:30:29 2009
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
      33,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x0a,
      43,   22,   22,   22, 0x0a,
      67,   22,   22,   22, 0x0a,
      86,   22,   22,   22, 0x0a,
     105,   22,   22,   22, 0x0a,
     129,   22,   22,   22, 0x0a,
     153,   22,   22,   22, 0x0a,
     168,   22,   22,   22, 0x0a,
     188,   22,   22,   22, 0x0a,
     213,   22,   22,   22, 0x0a,
     232,   22,   22,   22, 0x0a,
     254,   22,   22,   22, 0x0a,
     276,   22,   22,   22, 0x0a,
     293,   22,   22,   22, 0x0a,
     313,   22,   22,   22, 0x0a,
     334,   22,   22,   22, 0x0a,
     354,   22,   22,   22, 0x0a,
     375,   22,   22,   22, 0x0a,
     401,   22,   22,   22, 0x0a,
     421,   22,   22,   22, 0x0a,
     439,   22,   22,   22, 0x0a,
     468,   22,   22,   22, 0x0a,
     487,   22,   22,   22, 0x0a,
     507,   22,   22,   22, 0x0a,
     522,   22,   22,   22, 0x0a,
     545,   22,   22,   22, 0x0a,
     570,   22,   22,   22, 0x0a,
     589,   22,   22,   22, 0x0a,
     625,  612,   22,   22, 0x0a,
     666,  649,   22,   22, 0x0a,
     690,   22,   22,   22, 0x0a,
     719,   22,   22,   22, 0x0a,
     751,  744,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrActionSignalManager[] = {
    "FrActionSignalManager\0\0OnOpenImageAction()\0"
    "OnOpenDataStoreAction()\0OnSaveTabsAction()\0"
    "OnLoadTabsAction()\0OnSaveGraphTabsAction()\0"
    "OnLoadGraphTabsAction()\0OnExitAction()\0"
    "OnSaveToTabAction()\0OnSaveGraphToTabAction()\0"
    "OnNewLayerAction()\0OnDeleteLayerAction()\0"
    "OnChangeLayerAction()\0OnNewROIAction()\0"
    "OnViewSliceAction()\0OnViewMosaicAction()\0"
    "OnViewOrthoAction()\0OnResetImageAction()\0"
    "OnManipulatorToolAction()\0OnVoxelToolAction()\0"
    "OnRoiToolAction()\0OnVoxelSelectionToolAction()\0"
    "OnShowHelpAction()\0OnShowAboutAction()\0"
    "OnTestAction()\0OnLayerParamsChanged()\0"
    "OnLayerColormapChanged()\0OnRoiToolChanged()\0"
    "OnImageParamsChanged()\0currentValue\0"
    "OnLiveModeChanged(bool)\0currentTimePoint\0"
    "OnTimePointChanged(int)\0"
    "OnPreviousTimePointPressed()\0"
    "OnNextTimePointPressed()\0id,add\0"
    "OnGraphChanged(int,bool)\0"
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
        case 1: OnOpenDataStoreAction(); break;
        case 2: OnSaveTabsAction(); break;
        case 3: OnLoadTabsAction(); break;
        case 4: OnSaveGraphTabsAction(); break;
        case 5: OnLoadGraphTabsAction(); break;
        case 6: OnExitAction(); break;
        case 7: OnSaveToTabAction(); break;
        case 8: OnSaveGraphToTabAction(); break;
        case 9: OnNewLayerAction(); break;
        case 10: OnDeleteLayerAction(); break;
        case 11: OnChangeLayerAction(); break;
        case 12: OnNewROIAction(); break;
        case 13: OnViewSliceAction(); break;
        case 14: OnViewMosaicAction(); break;
        case 15: OnViewOrthoAction(); break;
        case 16: OnResetImageAction(); break;
        case 17: OnManipulatorToolAction(); break;
        case 18: OnVoxelToolAction(); break;
        case 19: OnRoiToolAction(); break;
        case 20: OnVoxelSelectionToolAction(); break;
        case 21: OnShowHelpAction(); break;
        case 22: OnShowAboutAction(); break;
        case 23: OnTestAction(); break;
        case 24: OnLayerParamsChanged(); break;
        case 25: OnLayerColormapChanged(); break;
        case 26: OnRoiToolChanged(); break;
        case 27: OnImageParamsChanged(); break;
        case 28: OnLiveModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 29: OnTimePointChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: OnPreviousTimePointPressed(); break;
        case 31: OnNextTimePointPressed(); break;
        case 32: OnGraphChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        }
        _id -= 33;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
