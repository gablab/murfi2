#include "FrLoadTabsCmd.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrXmlDefs.h"

// Qt stuff
#include "Qt/QFile.h"
#include "Qt/QTextStream.h"
#include "QtXml/QDom.h"

FrLoadTabsCmd::FrLoadTabsCmd()
: m_FileName(""), m_Document(0) {
    m_Document = new QDomDocument();
}

FrLoadTabsCmd::~FrLoadTabsCmd(){
    if(m_Document) delete m_Document;
}

bool FrLoadTabsCmd::Execute(){
    if(!this->GetMainController()) return false;

    return false;
}

bool FrLoadTabsCmd::LoadTabSettings(QDomElement& root, FrTabSettingsDocObj* tabs){
    return false;
}

void FrLoadTabsCmd::LoadSliceViewSettings(QDomElement& parent,  void* svSets){
}

void FrLoadTabsCmd::LoadMosaicViewSettings(QDomElement& parent, void* mvSets){
}

void FrLoadTabsCmd::LoadOrthoViewSettings(QDomElement& parent,  void* ovSets){
}

void FrLoadTabsCmd::LoadCameraSettings(QDomElement& parent, void* camSets){
}

void FrLoadTabsCmd::LoadLayeredImgSettings(QDomElement& parent, void* liSets){
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrLoadTabsCmd::CanUndo(){
    return false;
}

bool FrLoadTabsCmd::Undo(){
    return false;
}

bool FrLoadTabsCmd::Redo(){
    return false;
}