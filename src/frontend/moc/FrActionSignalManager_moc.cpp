/****************************************************************************
** Meta object code from reading C++ file 'FrActionSignalManager.h'
**
** Created: Mon 17. Nov 11:21:08 2008
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
      22,   10, // methods
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
     157,   22,   22,   22, 0x0a,
     179,   22,   22,   22, 0x0a,
     196,   22,   22,   22, 0x0a,
     216,   22,   22,   22, 0x0a,
     237,   22,   22,   22, 0x0a,
     257,   22,   22,   22, 0x0a,
     278,   22,   22,   22, 0x0a,
     304,   22,   22,   22, 0x0a,
     324,   22,   22,   22, 0x0a,
     342,   22,   22,   22, 0x0a,
     361,   22,   22,   22, 0x0a,
     381,   22,   22,   22, 0x0a,
     396,   22,   22,   22, 0x0a,
     419,   22,   22,   22, 0x0a,
     444,   22,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrActionSignalManager[] = {
    "FrActionSignalManager\0\0OnOpenImageAction()\0"
    "OnSaveTabsAction()\0OnLoadTabsAction()\0"
    "OnExitAction()\0OnSaveToTabAction()\0"
    "OnNewLayerAction()\0OnDeleteLayerAction()\0"
    "OnChangeLayerAction()\0OnNewROIAction()\0"
    "OnViewSliceAction()\0OnViewMosaicAction()\0"
    "OnViewOrthoAction()\0OnResetImageAction()\0"
    "OnManipulatorToolAction()\0OnVoxelToolAction()\0"
    "OnRoiToolAction()\0OnShowHelpAction()\0"
    "OnShowAboutAction()\0OnTestAction()\0"
    "OnLayerParamsChanged()\0OnLayerColormapChanged()\0"
    "OnRoiToolChanged()\0"
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
        case 5: OnNewLayerAction(); break;
        case 6: OnDeleteLayerAction(); break;
        case 7: OnChangeLayerAction(); break;
        case 8: OnNewROIAction(); break;
        case 9: OnViewSliceAction(); break;
        case 10: OnViewMosaicAction(); break;
        case 11: OnViewOrthoAction(); break;
        case 12: OnResetImageAction(); break;
        case 13: OnManipulatorToolAction(); break;
        case 14: OnVoxelToolAction(); break;
        case 15: OnRoiToolAction(); break;
        case 16: OnShowHelpAction(); break;
        case 17: OnShowAboutAction(); break;
        case 18: OnTestAction(); break;
        case 19: OnLayerParamsChanged(); break;
        case 20: OnLayerColormapChanged(); break;
        case 21: OnRoiToolChanged(); break;
        }
        _id -= 22;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
