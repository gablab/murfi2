#include "FrSaveTabsCmd.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrXmlDefs.h"

// Qt stuff
#include "Qt/QFile.h"
#include "Qt/QTextStream.h"
#include "QtXml/QDom.h"

// Some defines
#define TAB_SET_VERSION 1
#define DEF_NODE_INDENT 4
#define RENDERER_COUNT 3


FrSaveTabsCmd::FrSaveTabsCmd()
: m_FileName(""), m_Document(0) {
    m_Document = new QDomDocument();
}

FrSaveTabsCmd::~FrSaveTabsCmd(){
    if(m_Document) delete m_Document;
}

bool FrSaveTabsCmd::Execute(){
    if(!this->GetMainController()) return false;

    std::vector<FrDocumentObj*> tabSettings;
    FrMainDocument* mainDoc = this->GetMainController()->GetMainDocument();
    mainDoc->GetObjectsByType(tabSettings, FrDocumentObj::TabSettings);
    
    bool result = false;
    if(m_Document && !(m_FileName.isNull() || m_FileName.isEmpty())){
    
        // Add declaration and root element
        m_Document->clear();
        QDomProcessingInstruction declaration = 
            m_Document->createProcessingInstruction("xml", FR_XML_DECLARATION);

        QDomElement root = m_Document->createElement(FR_XML_TABSETS_ELEM);
        root.setAttribute(FR_XML_VERSION_ATTR, TAB_SET_VERSION);

        m_Document->appendChild(declaration);
        m_Document->appendChild(root);
        
        // Write to file
        QFile outFile(m_FileName);
        if (outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {

            // Add all settings
            for(int i = 0; i < tabSettings.size(); ++i){
                FrTabSettingsDocObj* tsDO = (FrTabSettingsDocObj*) tabSettings[i];
                this->SaveTabSettings(root, tsDO);
            }

            QTextStream out(&outFile);
            out.setCodec("UTF-8");
            m_Document->save(out, DEF_NODE_INDENT);
            result = true;
        }
    }
    return result;
}

bool FrSaveTabsCmd::SaveTabSettings(QDomElement& root, FrTabSettingsDocObj* tabs){
    if(!m_Document) return false;
    
    QDomElement tabSetsElem = m_Document->createElement(FR_XML_TABSETTINGS_ELEM);
    tabSetsElem.setAttribute(FR_XML_NAME_ATTR, tabs->GetName());
    tabSetsElem.setAttribute(FR_XML_DESCRIPTION_ATTR, tabs->GetDescription());
    tabSetsElem.setAttribute(FR_XML_ACTIVEVIEW_ATTR, int(tabs->GetActiveView()));
    
    this->SaveSliceViewSettings(tabSetsElem,  &tabs->GetSliceViewSettings());
    this->SaveMosaicViewSettings(tabSetsElem, &tabs->GetMosaicViewSettings());
    this->SaveOrthoViewSettings(tabSetsElem,  &tabs->GetOrthoViewSettings());

    root.appendChild(tabSetsElem);
    return true;
}

void FrSaveTabsCmd::SaveSliceViewSettings(QDomElement& parent,  void* svSets){
    ViewSettings* vs = (ViewSettings*)svSets;

    // slice number
    QDomElement sliceNumElem = m_Document->createElement(FR_XML_SLICENUM_ELEM);
    sliceNumElem.setAttribute(FR_XML_VALUE_ATTR, vs->SliceNumber);
    parent.appendChild(sliceNumElem);
        
    // cameras
    QDomElement camerasElem = m_Document->createElement(FR_XML_CAMS_ELEM);
    camerasElem.setAttribute(FR_XML_COUNT_ATTR, 1);
    SaveCameraSettings(camerasElem, &vs->CamSettings);
    parent.appendChild(camerasElem);

    // NOTE: second param is null for a while!
    SaveLayeredImgSettings(parent, 0L); 
}

void FrSaveTabsCmd::SaveMosaicViewSettings(QDomElement& parent, void* mvSets){
    ViewSettings* vs = (ViewSettings*)mvSets;

    // Slice number
    QDomElement sliceNumElem = m_Document->createElement(FR_XML_SLICENUM_ELEM);
    sliceNumElem.setAttribute(FR_XML_VALUE_ATTR, vs->SliceNumber);
    parent.appendChild(sliceNumElem);
        
    // cameras
    QDomElement camerasElem = m_Document->createElement(FR_XML_CAMS_ELEM);
    camerasElem.setAttribute(FR_XML_COUNT_ATTR, 1);
    SaveCameraSettings(camerasElem, &vs->CamSettings);
    parent.appendChild(camerasElem);

    // NOTE: second param is null for a while!
    SaveLayeredImgSettings(parent, 0L); 
}

void FrSaveTabsCmd::SaveOrthoViewSettings(QDomElement& parent,  void* ovSets){
    OViewSettings* vs = (OViewSettings*)ovSets;

    // Slice number
    QDomElement sliceNumElem = m_Document->createElement(FR_XML_SLICENUM_ELEM);
    sliceNumElem.setAttribute(FR_XML_CORONAL_ATTR, vs->CoronalSlice);
    sliceNumElem.setAttribute(FR_XML_SAGITAL_ATTR, vs->SagitalSlice);
    sliceNumElem.setAttribute(FR_XML_AXIAL_ATTR,   vs->AxialSlice);
    parent.appendChild(sliceNumElem);

    // cameras
    QDomElement camerasElem = m_Document->createElement(FR_XML_CAMS_ELEM);
    camerasElem.setAttribute(FR_XML_COUNT_ATTR, RENDERER_COUNT);
    for(int i=0; i < RENDERER_COUNT; ++i){
        SaveCameraSettings(camerasElem, &vs->CamSettings[i]);
    }
    parent.appendChild(camerasElem);

    // NOTE: second param is null for a while!
    SaveLayeredImgSettings(parent, 0L); 

}

void FrSaveTabsCmd::SaveCameraSettings(QDomElement& parent, void* camSets){
    CameraSettings* cs = (CameraSettings*)camSets;

    QDomElement camElem = m_Document->createElement(FR_XML_CAM_ELEM);
    {
        // Scale elem
        QDomElement scaleElem = m_Document->createElement(FR_XML_SCALE_ELEM);
        scaleElem.setAttribute(FR_XML_VALUE_ATTR, cs->Scale);
        camElem.appendChild(scaleElem);

        // Up vector
        QDomElement upElem = m_Document->createElement(FR_XML_VIEWUP_ELEM);
        upElem.setAttribute(FR_XML_X_ATTR, cs->ViewUp[0]);
        upElem.setAttribute(FR_XML_Y_ATTR, cs->ViewUp[1]);
        upElem.setAttribute(FR_XML_Z_ATTR, cs->ViewUp[2]);
        camElem.appendChild(upElem);

        // position
        QDomElement posElem = m_Document->createElement(FR_XML_POS_ELEM);
        posElem.setAttribute(FR_XML_X_ATTR, cs->Position[0]);
        posElem.setAttribute(FR_XML_Y_ATTR, cs->Position[1]);
        posElem.setAttribute(FR_XML_Z_ATTR, cs->Position[2]);
        camElem.appendChild(posElem);

        // Focal point
        QDomElement focusElem = m_Document->createElement(FR_XML_FOCUS_ELEM);
        focusElem.setAttribute(FR_XML_X_ATTR, cs->FocalPoint[0]);
        focusElem.setAttribute(FR_XML_Y_ATTR, cs->FocalPoint[1]);
        focusElem.setAttribute(FR_XML_Z_ATTR, cs->FocalPoint[2]);
        camElem.appendChild(focusElem);
    }
    parent.appendChild(camElem);
}

void FrSaveTabsCmd::SaveLayeredImgSettings(QDomElement& parent, void* liSets){
    QDomElement liElem = m_Document->createElement(FR_XML_LIMAGE_ELEM);
    liElem.setAttribute(FR_XML_LAYERCNT_ATTR, 1);
    parent.appendChild(liElem);

    // TODO: fox this when layered image will be implemented
    return;

    // For each layer in layeres of LayeredImage do
    {
        QDomElement layerElem = m_Document->createElement(FR_XML_LAYER_ELEM);
        layerElem.setAttribute(FR_XML_OPACITY_ATTR, 1.0);
        layerElem.setAttribute(FR_XML_VISIBLE_ATTR, 1);
        {
            // pxRange
            QDomElement prElem = m_Document->createElement(FR_XML_PXRANGE_ELEM);
            prElem.setAttribute(FR_XML_MINVAL_ATTR, 0);
            prElem.setAttribute(FR_XML_MAXVAL_ATTR, 255);
            layerElem.appendChild(prElem);

            // colorMap
            QDomElement cmElem = m_Document->createElement(FR_XML_CMAP_ELEM);
            cmElem.setAttribute(FR_XML_TYPE_ATTR, "multiColor");
            cmElem.setAttribute(FR_XML_THRESH_ATTR, 0.5);
            layerElem.appendChild(cmElem);

            // tbcSettings
            QDomElement tbcElem = m_Document->createElement(FR_XML_TBCSETS_ELEM);
            {
                // threshold
                QDomElement tElem = m_Document->createElement(FR_XML_THRESH_ELEM);
                tElem.setAttribute(FR_XML_VALUE_ATTR, 0.0);
                tbcElem.appendChild(tElem);
                // brightness
                QDomElement bElem = m_Document->createElement(FR_XML_BRIGHT_ELEM);
                bElem.setAttribute(FR_XML_VALUE_ATTR, 0.0);
                tbcElem.appendChild(bElem);
                // contrast
                QDomElement cElem = m_Document->createElement(FR_XML_CONTRAST_ELEM);
                cElem.setAttribute(FR_XML_VALUE_ATTR, 0.0);
                tbcElem.appendChild(cElem);
            }
            layerElem.appendChild(tbcElem);
        }
        liElem.appendChild(layerElem);
    }
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrSaveTabsCmd::CanUndo(){
    return false;
}

bool FrSaveTabsCmd::Undo(){
    return false;
}

bool FrSaveTabsCmd::Redo(){
    return false;
}