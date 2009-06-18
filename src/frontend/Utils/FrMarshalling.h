#ifndef FR_MARSHALLING
#define FR_MARSHALLING

#include "FrMacro.h"
#include "Qt/qaction.h"
#include "Qt/qevent.h"

class FrMarshallingEvent;

// Performes marshalling current event to main thread
// NOTE: It takes care about passed event so user do not need to delete it.
// Example:
// FrMarshallingEvent* event = new FrMarshallingEvent();
// MarshallToMainThread(event);
bool MarshalToMainThread(FrMarshallingEvent* event);

//
// Base class for all marshaling events
// Sometimes we have to make sure that code will be execurted in main thread.
// To do this we have to marshal execution to main thread.
// MarshallingEvent can help us here.
// Event cannot be marshalled to main thread itself but
// user have to use utility function: bool MarshallToMainThread(FrMarshallingEvent* event) 
class FrMarshallingEvent : public QEvent {
public:
    // Event identifier
    static int EventType;
public:
    FrMarshallingEvent();
    virtual ~FrMarshallingEvent(){}

    virtual void Execute() = 0;	
};

//
// This class is used for layer list widget updating
class FrLayerListWidget;
class FrLLWMarshalingEvent : public FrMarshallingEvent {
public:
    explicit FrLLWMarshalingEvent(FrLayerListWidget* llw);
    virtual void Execute();
private:
    FrLayerListWidget* m_layerListWidget;
};

// this class is used to marshal DataStore notification
class FrDataStore;
#include "RtDataID.h"
class FrDataStoreNotifyMarshallingEvent : public FrMarshallingEvent {
public:
    explicit FrDataStoreNotifyMarshallingEvent(FrDataStore* ds, const RtDataID &dataID);
    virtual void Execute();

private:
    FrDataStore* m_dataStore;
    RtDataID     m_dataID;
};

#endif
