#include "FrSaveTabsCmd.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrXmlDefs.h"

// Qt stuff
#include "Qt/qfile.h"
#include "Qt/qtextstream.h"
#include "QtXml/qdom.h"



// Some defines
#define DEF_NODE_INDENT 4
#define GET_HEX_COLOR_STRING(cl) ((cl == 0) ? QString("00") : QString().setNum(cl, 16))

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

        QDomElement root = m_Document->createElement(FR_XML_ROOT_ELEM);
        root.setAttribute(FR_XML_VERSION_ATTR, FR_TAB_SET_VERSION);

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

            outFile.close();
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
    root.appendChild(tabSetsElem);
    
    // save view settings
    QDomElement sliceElem = m_Document->createElement(FR_XML_SLICEVIEW_ELEM);
    this->SaveSliceViewSettings(sliceElem,  tabs->GetSliceViewSettings());
    tabSetsElem.appendChild(sliceElem);

    QDomElement mosaicElem = m_Document->createElement(FR_XML_MOSAICVIEW_ELEM);
    this->SaveMosaicViewSettings(mosaicElem, tabs->GetMosaicViewSettings());
    tabSetsElem.appendChild(mosaicElem);

    QDomElement orthoElem = m_Document->createElement(FR_XML_ORTHOVIEW_ELEM);
    this->SaveOrthoViewSettings(orthoElem,  tabs->GetOrthoViewSettings());
    tabSetsElem.appendChild(orthoElem);
    
    // save layered image settings
    QDomElement imagesElem = m_Document->createElement(FR_XML_LIS_ELEM);
    imagesElem.setAttribute(FR_XML_COUNT_ATTR, 1);
    SaveLayeredImageSettings(imagesElem, 0, tabs->GetImageLayer(), tabs->GetLayers());
    tabSetsElem.appendChild(imagesElem);

    return true;
}

void FrSaveTabsCmd::SaveSliceViewSettings(QDomElement& parent, FrSliceViewSettings* vs){
    // slice number
    QDomElement sliceNumElem = m_Document->createElement(FR_XML_SLICENUM_ELEM);
    sliceNumElem.setAttribute(FR_XML_VALUE_ATTR, vs->SliceNumber);
    parent.appendChild(sliceNumElem);

    // Active layer
    QDomElement layerNumElem = m_Document->createElement(FR_XML_LAYERNUM_ELEM);
    layerNumElem.setAttribute(FR_XML_VALUE_ATTR, vs->ActiveLayerID);
    parent.appendChild(layerNumElem);
    
    // cameras
    QDomElement camerasElem = m_Document->createElement(FR_XML_CAMS_ELEM);
    camerasElem.setAttribute(FR_XML_COUNT_ATTR, 1);
    SaveCameraSettings(camerasElem, 0, &vs->CamSettings);
    parent.appendChild(camerasElem);
}

void FrSaveTabsCmd::SaveMosaicViewSettings(QDomElement& parent, FrMosaicViewSettings* vs){
    // Active layer
    QDomElement layerNumElem = m_Document->createElement(FR_XML_LAYERNUM_ELEM);
    layerNumElem.setAttribute(FR_XML_VALUE_ATTR, vs->ActiveLayerID);
    parent.appendChild(layerNumElem);
    
    // cameras
    QDomElement camerasElem = m_Document->createElement(FR_XML_CAMS_ELEM);
    camerasElem.setAttribute(FR_XML_COUNT_ATTR, 1);
    SaveCameraSettings(camerasElem, 0, &vs->CamSettings);
    parent.appendChild(camerasElem);
}

void FrSaveTabsCmd::SaveOrthoViewSettings(QDomElement& parent, FrOrthoViewSettings* vs){
    // slice number
    QDomElement sliceNumElem = m_Document->createElement(FR_XML_SLICENUM_ELEM);
    sliceNumElem.setAttribute(FR_XML_CORONAL_ATTR, vs->SliceNumber[DEF_CORONAL]);
    sliceNumElem.setAttribute(FR_XML_SAGITAL_ATTR, vs->SliceNumber[DEF_SAGITAL]);
    sliceNumElem.setAttribute(FR_XML_AXIAL_ATTR,   vs->SliceNumber[DEF_AXIAL]);
    parent.appendChild(sliceNumElem);

    // Active layer
    QDomElement layerNumElem = m_Document->createElement(FR_XML_LAYERNUM_ELEM);
    layerNumElem.setAttribute(FR_XML_VALUE_ATTR, vs->ActiveLayerID);
    parent.appendChild(layerNumElem);
    
    // cameras
    QDomElement camerasElem = m_Document->createElement(FR_XML_CAMS_ELEM);
    camerasElem.setAttribute(FR_XML_COUNT_ATTR, ORTHO_VIEWS_CNT);
    for(int i=0; i < ORTHO_VIEWS_CNT; ++i){
        SaveCameraSettings(camerasElem, i, &vs->CamSettings[i]);
    }
    parent.appendChild(camerasElem);
}

void FrSaveTabsCmd::SaveCameraSettings(QDomElement& parent, int id, FrCameraSettings* camSets){
    QDomElement camElem = m_Document->createElement(FR_XML_CAM_ELEM);
    camElem.setAttribute(FR_XML_ID_ATTR, id);

    // Scale elem
    QDomElement scaleElem = m_Document->createElement(FR_XML_SCALE_ELEM);
    scaleElem.setAttribute(FR_XML_VALUE_ATTR, camSets->Scale);
    camElem.appendChild(scaleElem);

    // Up vector
    QDomElement upElem = m_Document->createElement(FR_XML_VIEWUP_ELEM);
    upElem.setAttribute(FR_XML_X_ATTR, camSets->ViewUp[0]);
    upElem.setAttribute(FR_XML_Y_ATTR, camSets->ViewUp[1]);
    upElem.setAttribute(FR_XML_Z_ATTR, camSets->ViewUp[2]);
    camElem.appendChild(upElem);

    // position
    QDomElement posElem = m_Document->createElement(FR_XML_POS_ELEM);
    posElem.setAttribute(FR_XML_X_ATTR, camSets->Position[0]);
    posElem.setAttribute(FR_XML_Y_ATTR, camSets->Position[1]);
    posElem.setAttribute(FR_XML_Z_ATTR, camSets->Position[2]);
    camElem.appendChild(posElem);

    // Focal point
    QDomElement focusElem = m_Document->createElement(FR_XML_FOCUS_ELEM);
    focusElem.setAttribute(FR_XML_X_ATTR, camSets->FocalPoint[0]);
    focusElem.setAttribute(FR_XML_Y_ATTR, camSets->FocalPoint[1]);
    focusElem.setAttribute(FR_XML_Z_ATTR, camSets->FocalPoint[2]);
    camElem.appendChild(focusElem);

    parent.appendChild(camElem);
}


void FrSaveTabsCmd::SaveLayeredImageSettings(QDomElement& parent, int id, 
                                             FrImageLayerSettings* mlSets, 
                                             std::vector<FrLayerSettings*>& olSets){
    // image
    QDomElement imageElem = m_Document->createElement(FR_XML_IMG_ELEM);
    imageElem.setAttribute(FR_XML_ID_ATTR, id);
    imageElem.setAttribute(FR_XML_NAME_ATTR, mlSets->Name);
    imageElem.setAttribute(FR_XML_OPACITY_ATTR, mlSets->Opacity);
    imageElem.setAttribute(FR_XML_VISIBLE_ATTR, (mlSets->Visibility ? 1 : 0) );
    parent.appendChild(imageElem);

    // Save tbc settings
    SaveTbcSettings(imageElem, &mlSets->TbcSettings);

    // Layers       // save colormap and roi settings with different functions?
    QDomElement layersElem = m_Document->createElement(FR_XML_LAYERS_ELEM);
    layersElem.setAttribute(FR_XML_COUNT_ATTR, olSets.size());
    std::vector<FrLayerSettings*>::iterator it, itEnd(olSets.end());
    for(it = olSets.begin(); it != itEnd; ++it){
        SaveLayerSettings(layersElem, (*it));
    }
    imageElem.appendChild(layersElem);
}

void FrSaveTabsCmd::SaveLayerSettings(QDomElement& parent, FrLayerSettings* liSets){
    QDomElement layerElem = m_Document->createElement(FR_XML_LAYER_ELEM);
//    layerElem.setAttribute(FR_XML_ID_ATTR, liSets->ID);
    
    FrLayerSettings::LTypes ltype = liSets->GetType();

    layerElem.setAttribute(FR_XML_TYPE_ATTR, (int)ltype);
    layerElem.setAttribute(FR_XML_NAME_ATTR, liSets->Name);
    layerElem.setAttribute(FR_XML_OPACITY_ATTR, liSets->Opacity);
    layerElem.setAttribute(FR_XML_VISIBLE_ATTR, (liSets->Visibility ? 1 : 0) );
    layerElem.setAttribute(FR_XML_NAME_ATTR, liSets->Name);

    switch (ltype){
        case FrLayerSettings::LRoi:
            // do nothing
            break;
        case FrLayerSettings::LColormap:
            FrColormapLayerSettings* cmlSets = (FrColormapLayerSettings*)liSets;

            // pxRange
            QDomElement prElem = m_Document->createElement(FR_XML_PXRANGE_ELEM);
            prElem.setAttribute(FR_XML_MINVAL_ATTR, cmlSets->ColormapSettings.MinValue);
            prElem.setAttribute(FR_XML_MAXVAL_ATTR, cmlSets->ColormapSettings.MaxValue);
            layerElem.appendChild(prElem);

            // colorMap
            QDomElement cmElem = m_Document->createElement(FR_XML_CMAP_ELEM);
            switch(cmlSets->ColormapSettings.Type){
                case FrColormapSettings::MultiColor:
                    cmElem.setAttribute(FR_XML_TYPE_ATTR, FR_CMTYPE_MULTI);
                    break;
                case FrColormapSettings::SingleColor:
                    cmElem.setAttribute(FR_XML_TYPE_ATTR, FR_CMTYPE_SINGLE);
                    break;
                default:
                    // Do nothing here
                    break;
            }
            cmElem.setAttribute(FR_XML_MIDVALUE_ATTR, cmlSets->ColormapSettings.MidValue);
	        cmElem.setAttribute(FR_XML_THRESH_ATTR, cmlSets->ColormapSettings.Threshold);

            // Save color value
            QColor color = cmlSets->ColormapSettings.Color;
            QString colorValue = "#";
            colorValue += GET_HEX_COLOR_STRING(color.red());
            colorValue += GET_HEX_COLOR_STRING(color.green());
            colorValue += GET_HEX_COLOR_STRING(color.blue());
            cmElem.setAttribute(FR_XML_COLOR_ATTR, colorValue);

            layerElem.appendChild(cmElem);

            // TBC
            SaveTbcSettings(layerElem, &cmlSets->TbcSettings);
            break;
    }

    parent.appendChild(layerElem);
}

void FrSaveTabsCmd::SaveTbcSettings(QDomElement& parent, FrTbcSettings* tbcSets){

    QDomElement tbcElem = m_Document->createElement(FR_XML_TBCSETS_ELEM);

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

    parent.appendChild(tbcElem);
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
