/****************************************************************************
** Meta object code from reading C++ file 'FrActionSignalManager.h'
**
** Created: Tue Jun 9 12:53:49 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Utils/FrActionSignalManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrActionSignalManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrActionSignalManager[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      38,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x0a,
      43,   22,   22,   22, 0x0a,
      64,   22,   22,   22, 0x0a,
      88,   22,   22,   22, 0x0a,
     107,   22,   22,   22, 0x0a,
     126,   22,   22,   22, 0x0a,
     150,   22,   22,   22, 0x0a,
     174,   22,   22,   22, 0x0a,
     189,   22,   22,   22, 0x0a,
     209,   22,   22,   22, 0x0a,
     234,   22,   22,   22, 0x0a,
     253,   22,   22,   22, 0x0a,
     275,   22,   22,   22, 0x0a,
     297,   22,   22,   22, 0x0a,
     314,   22,   22,   22, 0x0a,
     334,   22,   22,   22, 0x0a,
     355,   22,   22,   22, 0x0a,
     375,   22,   22,   22, 0x0a,
     396,   22,   22,   22, 0x0a,
     422,   22,   22,   22, 0x0a,
     442,   22,   22,   22, 0x0a,
     460,   22,   22,   22, 0x0a,
     489,   22,   22,   22, 0x0a,
     508,   22,   22,   22, 0x0a,
     528,   22,   22,   22, 0x0a,
     543,   22,   22,   22, 0x0a,
     566,   22,   22,   22, 0x0a,
     595,  591,   22,   22, 0x0a,
     623,   22,   22,   22, 0x0a,
     642,   22,   22,   22, 0x0a,
     662,   22,   22,   22, 0x0a,
     698,  685,   22,   22, 0x0a,
     739,  722,   22,   22, 0x0a,
     763,   22,   22,   22, 0x0a,
     792,   22,   22,   22, 0x0a,
     828,  817,   22,   22, 0x0a,
     861,   22,   22,   22, 0x0a,
     890,  886,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrActionSignalManager[] = {
    "FrActionSignalManager\0\0OnOpenImageAction()\0"
    "OnOpenConfigAction()\0OnOpenDataStoreAction()\0"
    "OnSaveTabsAction()\0OnLoadTabsAction()\0"
    "OnSaveGraphTabsAction()\0OnLoadGraphTabsAction()\0"
    "OnExitAction()\0OnSaveToTabAction()\0"
    "OnSaveGraphToTabAction()\0OnNewLayerAction()\0"
    "OnDeleteLayerAction()\0OnChangeLayerAction()\0"
    "OnNewROIAction()\0OnViewSliceAction()\0"
    "OnViewMosaicAction()\0OnViewOrthoAction()\0"
    "OnResetImageAction()\0OnManipulatorToolAction()\0"
    "OnVoxelToolAction()\0OnRoiToolAction()\0"
    "OnVoxelSelectionToolAction()\0"
    "OnShowHelpAction()\0OnShowAboutAction()\0"
    "OnTestAction()\0OnLayerParamsChanged()\0"
    "OnLayerColormapChanged()\0inc\0"
    "OnLayerPositionChanged(int)\0"
    "OnRoiToolChanged()\0OnClearCurrentRoi()\0"
    "OnImageParamsChanged()\0currentValue\0"
    "OnLiveModeChanged(bool)\0currentTimePoint\0"
    "OnTimePointChanged(int)\0"
    "OnPreviousTimePointPressed()\0"
    "OnNextTimePointPressed()\0gID,id,add\0"
    "OnGraphChanged(ulong,ulong,bool)\0"
    "OnNewGraphWidgetAction()\0gID\0"
    "OnDeleteGraphWidgetAction(ulong)\0"
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
        case 1: OnOpenConfigAction(); break;
        case 2: OnOpenDataStoreAction(); break;
        case 3: OnSaveTabsAction(); break;
        case 4: OnLoadTabsAction(); break;
        case 5: OnSaveGraphTabsAction(); break;
        case 6: OnLoadGraphTabsAction(); break;
        case 7: OnExitAction(); break;
        case 8: OnSaveToTabAction(); break;
        case 9: OnSaveGraphToTabAction(); break;
        case 10: OnNewLayerAction(); break;
        case 11: OnDeleteLayerAction(); break;
        case 12: OnChangeLayerAction(); break;
        case 13: OnNewROIAction(); break;
        case 14: OnViewSliceAction(); break;
        case 15: OnViewMosaicAction(); break;
        case 16: OnViewOrthoAction(); break;
        case 17: OnResetImageAction(); break;
        case 18: OnManipulatorToolAction(); break;
        case 19: OnVoxelToolAction(); break;
        case 20: OnRoiToolAction(); break;
        case 21: OnVoxelSelectionToolAction(); break;
        case 22: OnShowHelpAction(); break;
        case 23: OnShowAboutAction(); break;
        case 24: OnTestAction(); break;
        case 25: OnLayerParamsChanged(); break;
        case 26: OnLayerColormapChanged(); break;
        case 27: OnLayerPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: OnRoiToolChanged(); break;
        case 29: OnClearCurrentRoi(); break;
        case 30: OnImageParamsChanged(); break;
        case 31: OnLiveModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 32: OnTimePointChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: OnPreviousTimePointPressed(); break;
        case 34: OnNextTimePointPressed(); break;
        case 35: OnGraphChanged((*reinterpret_cast< ulong(*)>(_a[1])),(*reinterpret_cast< ulong(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 36: OnNewGraphWidgetAction(); break;
        case 37: OnDeleteGraphWidgetAction((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        }
        _id -= 38;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
