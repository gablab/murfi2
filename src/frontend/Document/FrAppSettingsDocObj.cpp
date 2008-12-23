#include "FrAppSettingsDocObj.h"

#define DEFAULT_LIVE_MODE true

FrAppSettingsDocObj::FrAppSettingsDocObj()
: m_LiveMode(DEFAULT_LIVE_MODE){

}

FrAppSettingsDocObj::~FrAppSettingsDocObj(){

}

void FrAppSettingsDocObj::OnAdd(FrDocument* doc){

    // NOTE : Do nothing in base class
}

void FrAppSettingsDocObj::OnRemove(FrDocument* doc){

    // NOTE : Do nothing in base class 
}

FrAppSettingsDocObj::ObjTypes FrAppSettingsDocObj::GetType(){

    return FrAppSettingsDocObj::AppSettings;
}