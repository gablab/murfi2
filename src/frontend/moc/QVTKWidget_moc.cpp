/****************************************************************************
** Meta object code from reading C++ file 'QVTKWidget.h'
**
** Created: Tue 23. Dec 21:21:16 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/QVTKWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QVTKWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QVTKWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       2,   40, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x05,
      43,   11,   11,   11, 0x05,
      62,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      81,   11,   11,   11, 0x0a,
     106,   11,   11,   11, 0x0a,
     125,   11,   11,   11, 0x08,

 // properties: name, type, flags
     151,  146, 0x01095103,
     185,  178, 0x06095103,

       0        // eod
};

static const char qt_meta_stringdata_QVTKWidget[] = {
    "QVTKWidget\0\0event\0mouseEvent(QMouseEvent*)\0"
    "cachedImageDirty()\0cachedImageClean()\0"
    "markCachedImageAsDirty()\0saveImageToCache()\0"
    "internalMacFixRect()\0bool\0"
    "automaticImageCacheEnabled\0double\0"
    "maxRenderRateForImageCache\0"
};

const QMetaObject QVTKWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QVTKWidget,
      qt_meta_data_QVTKWidget, 0 }
};

const QMetaObject *QVTKWidget::metaObject() const
{
    return &staticMetaObject;
}

void *QVTKWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QVTKWidget))
	return static_cast<void*>(const_cast< QVTKWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int QVTKWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: mouseEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 1: cachedImageDirty(); break;
        case 2: cachedImageClean(); break;
        case 3: markCachedImageAsDirty(); break;
        case 4: saveImageToCache(); break;
        case 5: internalMacFixRect(); break;
        }
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = isAutomaticImageCacheEnabled(); break;
        case 1: *reinterpret_cast< double*>(_v) = maxRenderRateForImageCache(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setAutomaticImageCacheEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 1: setMaxRenderRateForImageCache(*reinterpret_cast< double*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QVTKWidget::mouseEvent(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QVTKWidget::cachedImageDirty()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QVTKWidget::cachedImageClean()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
static const uint qt_meta_data_QVTKInteractor[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QVTKInteractor[] = {
    "QVTKInteractor\0\0TimerEvent()\0"
};

const QMetaObject QVTKInteractor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QVTKInteractor,
      qt_meta_data_QVTKInteractor, 0 }
};

const QMetaObject *QVTKInteractor::metaObject() const
{
    return &staticMetaObject;
}

void *QVTKInteractor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QVTKInteractor))
	return static_cast<void*>(const_cast< QVTKInteractor*>(this));
    if (!strcmp(_clname, "vtkRenderWindowInteractor"))
	return static_cast< vtkRenderWindowInteractor*>(const_cast< QVTKInteractor*>(this));
    return QObject::qt_metacast(_clname);
}

int QVTKInteractor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: TimerEvent(); break;
        }
        _id -= 1;
    }
    return _id;
}
