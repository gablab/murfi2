#include "FrMarshalling.h"
#include "FrLayerListWidget.h"

#include "FrApplication.h"
#include "FrDataStore.h"

int FrMarshallingEvent::EventType = QEvent::registerEventType();

bool MarshalToMainThread(FrMarshallingEvent* event){
    // Post event to main window immediatly
    QObject* target = (QObject*)FrApplication::MainWindow;
    if(target) {
        // assume that Qt will take care about event itself
        QApplication::postEvent(target, (QEvent*)event, Qt::HighEventPriority);
        return true; 
    }
    delete event;
    return false;
}

//
// FrMarshallingEvent implementation
//
FrMarshallingEvent::FrMarshallingEvent()
    : QEvent(QEvent::Type(FrMarshallingEvent::EventType)) {
}

//
// FrLLWMarshalingEvent implementation
//
FrLLWMarshalingEvent::FrLLWMarshalingEvent(FrLayerListWidget* llw)
    : FrMarshallingEvent() {
    m_layerListWidget = llw;
}

void FrLLWMarshalingEvent::Execute() {
    if(m_layerListWidget) {
        m_layerListWidget->OnUpdate();
    }
}

//
// FrDataStoreNotifyMarshallingEvent implementat
//
FrDataStoreNotifyMarshallingEvent::
FrDataStoreNotifyMarshallingEvent(FrDataStore* ds, const RtDataID &dataID) {
    m_dataStore = ds;
    m_dataID = RtDataID(dataID);
}

void FrDataStoreNotifyMarshallingEvent::Execute(){

    if(m_dataStore) {
        m_dataStore->OnNotify(m_dataID);
    }
}
