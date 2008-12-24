/****************************************************************************
** Meta object code from reading C++ file 'FrActionSignalManager.h'
**
** Created: Wed Dec 24 17:47:44 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Utils/FrActionSignalManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrActionSignalManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrActionSignalManager[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x0a,
      43,   22,   22,   22, 0x0a,
      67,   22,   22,   22, 0x0a,
      86,   22,   22,   22, 0x0a,
     105,   22,   22,   22, 0x0a,
     120,   22,   22,   22, 0x0a,
     140,   22,   22,   22, 0x0a,
     159,   22,   22,   22, 0x0a,
     181,   22,   22,   22, 0x0a,
     203,   22,   22,   22, 0x0a,
     220,   22,   22,   22, 0x0a,
     240,   22,   22,   22, 0x0a,
     261,   22,   22,   22, 0x0a,
     281,   22,   22,   22, 0x0a,
     302,   22,   22,   22, 0x0a,
     328,   22,   22,   22, 0x0a,
     348,   22,   22,   22, 0x0a,
     366,   22,   22,   22, 0x0a,
     395,   22,   22,   22, 0x0a,
     414,   22,   22,   22, 0x0a,
     434,   22,   22,   22, 0x0a,
     449,   22,   22,   22, 0x0a,
     472,   22,   22,   22, 0x0a,
     497,   22,   22,   22, 0x0a,
     529,  516,   22,   22, 0x0a,
     570,  553,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrActionSignalManager[] = {
    "FrActionSignalManager\0\0OnOpenImageAction()\0"
    "OnOpenDataStoreAction()\0OnSaveTabsAction()\0"
    "OnLoadTabsAction()\0OnExitAction()\0"
    "OnSaveToTabAction()\0OnNewLayerAction()\0"
    "OnDeleteLayerAction()\0OnChangeLayerAction()\0"
    "OnNewROIAction()\0OnViewSliceAction()\0"
    "OnViewMosaicAction()\0OnViewOrthoAction()\0"
    "OnResetImageAction()\0OnManipulatorToolAction()\0"
    "OnVoxelToolAction()\0OnRoiToolAction()\0"
    "OnVoxelSelectionToolAction()\0"
    "OnShowHelpAction()\0OnShowAboutAction()\0"
    "OnTestAction()\0OnLayerParamsChanged()\0"
    "OnLayerColormapChanged()\0OnRoiToolChanged()\0"
    "currentValue\0OnLiveModeChanged(bool)\0"
    "currentTimePoint\0OnTimePointChanged(int)\0"
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
        case 17: OnVoxelSelectionToolAction(); break;
        case 18: OnShowHelpAction(); break;
        case 19: OnShowAboutAction(); break;
        case 20: OnTestAction(); break;
        case 21: OnLayerParamsChanged(); break;
        case 22: OnLayerColormapChanged(); break;
        case 23: OnRoiToolChanged(); break;
        case 24: OnLiveModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: OnTimePointChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 26;
    }
    return _id;
}
