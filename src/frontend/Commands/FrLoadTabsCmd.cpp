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
    if(!(m_FileName.isNull() || m_FileName.isEmpty())) return false;
    m_tabSets.clear();

    // Load Dom document
    QFile inFile(m_FileName);
    if(inFile.open(QIODevice::ReadOnly) && m_Document->setContent(&inFile)){
        // get root
        QDomElement root = m_Document->documentElement();
        if(root.tagName() != FR_XML_ROOT_ELEM ||
          !root.hasAttribute(FR_XML_VERSION_ATTR)) return false;
        
        bool isVersionOk = false;
        int version = root.attribute(FR_XML_VERSION_ATTR).toInt(&isVersionOk);
        if(!isVersionOk || version != FR_TAB_SET_VERSION) return false;

        // if root found and version is ok then load settings
        QDomNode node = root.firstChild();
        while(!node.isNull()){
            QDomElement tabSetElem = node.toElement();
            if(!tabSetElem.isNull() && 
                tabSetElem.tagName() == FR_XML_TABSETTINGS_ELEM){
                // Create tab settings
                FrTabSettingsDocObj* tsDO = new FrTabSettingsDocObj();
                if(LoadTabSettings(tabSetElem, tsDO)){
                    m_tabSets.push_back(tsDO);
                }
                else {
                    delete tsDO;
                }
            }
            node = node.nextSibling();
        }

        // Add all tabSets to document
        if(m_tabSets.size() > 0){
            FrMainDocument* mainDoc = this->GetMainController()->GetMainDocument();
            TabSettingsVec::iterator it, itEnd(m_tabSets.end());
            for(it = m_tabSets.begin(); it != itEnd; ++it){
                mainDoc->Add(*it);
            }
        }
        else {
            return false;
        }

        // Close file if need to
        if(inFile.isOpen()){
            inFile.close();
        }
    }
    return true;
}

bool FrLoadTabsCmd::LoadTabSettings(QDomElement& elem, FrTabSettingsDocObj* tabs){
    if(!elem.hasChildNodes()) return false;

    //// Read all attributes
    //if(!elem.hasAttribute(FR_XML_NAME_ATTR)) return false;
    //tabs->SetName(elem.attribute(FR_XML_NAME_ATTR));

    //if(!elem.hasAttribute(FR_XML_DESCRIPTION_ATTR)) return false;
    //tabs->SetDescription(elem.attribute(FR_XML_DESCRIPTION_ATTR));

    //bool isActiveViewOk = false;
    //if(!elem.hasAttribute(FR_XML_ACTIVEVIEW_ATTR)) return false;
    //int activeView = elem.attribute(FR_XML_ACTIVEVIEW_ATTR).toInt(&isActiveViewOk);

    //if(!isActiveViewOk) return false;
    //tabs->SetActiveView((ActiveView::View)activeView);

    //bool hasSlice, hasMosaic, hasOrtho;
    //hasSlice = hasMosaic = hasOrtho = false;

    //QDomElement viewElem = elem.firstChildElement();
    //while(!viewElem.isNull()){
    //    if(viewElem.tagName() == FR_XML_SLICENUM_ELEM){
    //        hasSlice = LoadSliceViewSettings(viewElem, &tabs->GetSliceViewSettings());
    //        if(!hasSlice) return false;
    //    }
    //    else if(viewElem.tagName() == FR_XML_MOSAICVIEW_ELEM){
    //        hasMosaic = LoadMosaicViewSettings(viewElem, &tabs->GetMosaicViewSettings());
    //        if(!hasMosaic) return false;
    //    }
    //    else if(viewElem.tagName() == FR_XML_ORTHOVIEW_ELEM){
    //        hasOrtho = LoadOrthoViewSettings(viewElem, &tabs->GetOrthoViewSettings());
    //        if(!hasOrtho) return false;
    //    }
    //    viewElem = viewElem.nextSiblingElement();
    //}

    //return (hasSlice && hasMosaic && hasOrtho);
    return false;
}

bool FrLoadTabsCmd::LoadSliceViewSettings(QDomElement& elem,  void* svSets){
    //if(!elem.hasChildNodes()) return false;

    //ViewSettings* vs = (ViewSettings*)svSets;

    //bool hasSliceNum, hasCamera, hasLayeredImage;
    //hasSliceNum = hasCamera = hasLayeredImage = false;

    //// Load settings
    //QDomElement childElem = elem.firstChildElement();
    //while(!childElem.isNull()){
    //    if(childElem.tagName() == FR_XML_SLICENUM_ELEM){
    //        // Load slice number
    //        if(!childElem.hasAttribute(FR_XML_VALUE_ATTR)) return false;
    //        vs->SliceNumber = childElem.attribute(FR_XML_VALUE_ATTR).toInt(&hasSliceNum);
    //        if(!hasSliceNum) return false;
    //    }
    //    else if(childElem.tagName() == FR_XML_CAMS_ELEM){
    //        // Load camera settings for slice view
    //        if(!childElem.hasAttribute(FR_XML_COUNT_ATTR)) return false;
    //        int count = childElem.attribute(FR_XML_COUNT_ATTR).toInt(&hasCamera);
    //        if(!hasCamera || count != 1) return false;

    //        QDomElement camElem = childElem.firstChildElement();
    //        if(camElem.isNull()) return false;

    //        hasCamera = LoadCameraSettings(camElem, &vs->CamSettings);
    //        if(!hasCamera) return false;
    //    }
    //    else if(childElem.tagName() == FR_XML_LIMAGE_ELEM){
    //        // Load layered image settings
    //        // NOTE: second param is 0L for a while!
    //        hasLayeredImage = LoadLayeredImgSettings(elem, 0L);
    //        if(!hasLayeredImage) return false;
    //    }
    //    childElem = childElem.nextSiblingElement();
    //}
    //return (hasSliceNum && hasCamera && hasLayeredImage);
    return false;
}

bool FrLoadTabsCmd::LoadMosaicViewSettings(QDomElement& elem, void* mvSets){
    //if(!elem.hasChildNodes()) return false;

    //ViewSettings* vs = (ViewSettings*)mvSets;

    //bool hasSliceNum, hasCamera, hasLayeredImage;
    //hasSliceNum = hasCamera = hasLayeredImage = false;

    //// Load settings
    //QDomElement childElem = elem.firstChildElement();
    //while(!childElem.isNull()){
    //    if(childElem.tagName() == FR_XML_SLICENUM_ELEM){
    //        // Load slice number
    //        if(!childElem.hasAttribute(FR_XML_VALUE_ATTR)) return false;
    //        vs->SliceNumber = childElem.attribute(FR_XML_VALUE_ATTR).toInt(&hasSliceNum);
    //        if(!hasSliceNum) return false;
    //    }
    //    else if(childElem.tagName() == FR_XML_CAMS_ELEM){
    //        // Load camera settings for slice view
    //        if(!childElem.hasAttribute(FR_XML_COUNT_ATTR)) return false;
    //        int count = childElem.attribute(FR_XML_COUNT_ATTR).toInt(&hasCamera);
    //        if(!hasCamera || count != 1) return false;

    //        QDomElement camElem = childElem.firstChildElement();
    //        if(camElem.isNull()) return false;

    //        hasCamera = LoadCameraSettings(camElem, &vs->CamSettings);
    //        if(!hasCamera) return false;
    //    }
    //    else if(childElem.tagName() == FR_XML_LIMAGE_ELEM){
    //        // Load layered image settings
    //        // NOTE: second param is 0L for a while!
    //        hasLayeredImage = LoadLayeredImgSettings(elem, 0L);
    //        if(!hasLayeredImage) return false;
    //    }
    //    childElem = childElem.nextSiblingElement();
    //}
    //return (hasSliceNum && hasCamera && hasLayeredImage);
    return false;
}

bool FrLoadTabsCmd::LoadOrthoViewSettings(QDomElement& elem,  void* ovSets){
    if(!elem.hasChildNodes()) return false;

    //OViewSettings* vs = (OViewSettings*)ovSets;

    //bool hasSliceNum, hasCamera, hasLayeredImage;
    //hasSliceNum = hasCamera = hasLayeredImage = false;

    //// Load settings
    //QDomElement childElem = elem.firstChildElement();
    //while(!childElem.isNull()){
    //    if(childElem.tagName() == FR_XML_SLICENUM_ELEM){
    //        // Load slice numbers
    //        char* attrNames[] = {FR_XML_CORONAL_ATTR, 
    //                             FR_XML_SAGITAL_ATTR, 
    //                             FR_XML_AXIAL_ATTR };

    //        int* attributes[] = { &vs->CoronalSlice,
    //                              &vs->SagitalSlice,
    //                              &vs->AxialSlice };

    //        for(int i=0; i < 3; ++i){
    //            if(!childElem.hasAttribute(attrNames[i])) return false;
    //            (*(attributes[i])) = childElem.attribute(attrNames[i]).toInt(&hasSliceNum);
    //            if(!hasSliceNum) return false;
    //        }
    //    }
    //    else if(childElem.tagName() == FR_XML_CAMS_ELEM){
    //        // Load camera settings for slice view
    //        if(!childElem.hasAttribute(FR_XML_COUNT_ATTR)) return false;
    //        int count = childElem.attribute(FR_XML_COUNT_ATTR).toInt(&hasCamera);
    //        if(!hasCamera || count != 3) return false;

    //        // Load three cameras
    //        QDomElement camElem;
    //        for(int i=0; i < 3; ++i){
    //            QDomElement camElem = (i == 0) ? 
    //                childElem.firstChildElement() : camElem.nextSiblingElement();
    //            
    //            if(camElem.isNull()) return false;

    //            hasCamera = LoadCameraSettings(camElem, &vs->CamSettings[i]);
    //            if(!hasCamera) return false;
    //        }
    //    }
    //    else if(childElem.tagName() == FR_XML_LIMAGE_ELEM){
    //        // Load layered image settings
    //        // NOTE: second param is 0L for a while!
    //        hasLayeredImage = LoadLayeredImgSettings(elem, 0L);
    //        if(!hasLayeredImage) return false;
    //    }
    //    childElem = childElem.nextSiblingElement();
    //}
    //return (hasSliceNum && hasCamera && hasLayeredImage);
    return false;
}

bool FrLoadTabsCmd::LoadCameraSettings(QDomElement& elem, void* camSets){
    //if(elem.tagName() != FR_XML_CAM_ELEM) return false;
    //CameraSettings* cs = (CameraSettings*)camSets;

    //// read scale
    //bool hasScale, hasUp, hasPos, hasFP;
    //hasScale = hasUp = hasPos = hasFP = false;
    //
    //QDomElement childElem = elem.firstChildElement();
    //while(!childElem.isNull()){
    //    if(childElem.tagName() == FR_XML_SCALE_ELEM){
    //        // Read scale
    //        if(!childElem.hasAttribute(FR_XML_VALUE_ATTR)) return false;
    //        cs->Scale = childElem.attribute(FR_XML_VALUE_ATTR).toDouble(&hasScale);
    //        if(!hasScale) return false;
    //    }
    //    else if(childElem.tagName() == FR_XML_VIEWUP_ELEM){
    //        hasUp = LoadAttrValuesXYZ(childElem, cs->ViewUp);
    //        if(!hasUp) return false;
    //    }
    //    else if(childElem.tagName() == FR_XML_POS_ELEM){
    //        hasPos = LoadAttrValuesXYZ(childElem, cs->Position);
    //        if(!hasPos) return false;
    //    }
    //    else if(childElem.tagName() == FR_XML_FOCUS_ELEM){
    //        hasFP = LoadAttrValuesXYZ(childElem, cs->FocalPoint);
    //        if(!hasFP) return false;
    //    }
    //    childElem = childElem.nextSiblingElement();
    //}   
    //return (hasScale && hasUp && hasPos && hasFP);
    return false;
}

bool FrLoadTabsCmd::LoadLayeredImgSettings(QDomElement& elem, void* liSets){
    return true;
}

bool FrLoadTabsCmd::LoadAttrValuesXYZ(QDomElement& elem, double* vec){
    // Check input
    bool hasRequiredAttributes = elem.hasAttribute(FR_XML_X_ATTR) &&
        elem.hasAttribute(FR_XML_Y_ATTR) && elem.hasAttribute(FR_XML_Z_ATTR);
    if(!hasRequiredAttributes) return false;
    
    // load values
    bool isOk = false;
    vec[0] = elem.attribute(FR_XML_X_ATTR).toDouble(&isOk);
    if(!isOk) return false;

    vec[1] = elem.attribute(FR_XML_X_ATTR).toDouble(&isOk);
    if(!isOk) return false;

    vec[2] = elem.attribute(FR_XML_X_ATTR).toDouble(&isOk);
    return isOk;
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