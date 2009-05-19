/****************************************************************************
** Meta object code from reading C++ file 'FrPlayControlWidget.h'
**
** Created: Mon May 18 12:31:44 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrPlayControlWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrPlayControlWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrPlayControlWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      26,   21,   20,   20, 0x05,
      48,   20,   20,   20, 0x05,
      55,   20,   20,   20, 0x05,
      63,   20,   20,   20, 0x05,
      71,   20,   20,   20, 0x05,
      82,   20,   20,   20, 0x05,
      89,   20,   20,   20, 0x05,
      99,   20,   20,   20, 0x05,
     110,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
     125,   20,   20,   20, 0x08,
     145,   20,   20,   20, 0x08,
     161,   20,   20,   20, 0x08,
     178,   20,   20,   20, 0x08,
     195,   20,   20,   20, 0x08,
     215,   20,   20,   20, 0x08,
     231,   20,   20,   20, 0x08,
     249,   20,   20,   20, 0x08,
     267,   20,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrPlayControlWidget[] = {
    "FrPlayControlWidget\0\0isOn\0"
    "LiveModeChanged(bool)\0Play()\0Pause()\0"
    "Reset()\0Previous()\0Next()\0Speedup()\0"
    "Slowdown()\0GotoNextStep()\0OnLiveModeChanged()\0"
    "OnPlayPressed()\0OnPausePressed()\0"
    "OnResetPressed()\0OnPreviousPressed()\0"
    "OnNextPressed()\0OnSlowerPressed()\0"
    "OnFasterPressed()\0UpdateButtons()\0"
};

const QMetaObject FrPlayControlWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FrPlayControlWidget,
      qt_meta_data_FrPlayControlWidget, 0 }
};

const QMetaObject *FrPlayControlWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FrPlayControlWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrPlayControlWidget))
        return static_cast<void*>(const_cast< FrPlayControlWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int FrPlayControlWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: LiveModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: Play(); break;
        case 2: Pause(); break;
        case 3: Reset(); break;
        case 4: Previous(); break;
        case 5: Next(); break;
        case 6: Speedup(); break;
        case 7: Slowdown(); break;
        case 8: GotoNextStep(); break;
        case 9: OnLiveModeChanged(); break;
        case 10: OnPlayPressed(); break;
        case 11: OnPausePressed(); break;
        case 12: OnResetPressed(); break;
        case 13: OnPreviousPressed(); break;
        case 14: OnNextPressed(); break;
        case 15: OnSlowerPressed(); break;
        case 16: OnFasterPressed(); break;
        case 17: UpdateButtons(); break;
        }
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void FrPlayControlWidget::LiveModeChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FrPlayControlWidget::Play()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void FrPlayControlWidget::Pause()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void FrPlayControlWidget::Reset()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void FrPlayControlWidget::Previous()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void FrPlayControlWidget::Next()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void FrPlayControlWidget::Speedup()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void FrPlayControlWidget::Slowdown()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void FrPlayControlWidget::GotoNextStep()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}
QT_END_MOC_NAMESPACE
