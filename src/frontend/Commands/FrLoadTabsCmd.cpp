#include "FrLoadTabsCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrTabSettingsDocObj.h"
#include "FrXmlDefs.h"
#include "FrUtils.h"
#include "FrViewDocObj.h"

// Qt stuff
#include "Qt/qfile.h"
#include "Qt/qtextstream.h"
#include "QtXml/qdom.h"
#include "Qt/qmessagebox.h"



FrLoadTabsCmd::FrLoadTabsCmd()
: m_FileName(""), m_Document(0) {
    m_Document = new QDomDocument();
}

FrLoadTabsCmd::~FrLoadTabsCmd(){
    if(m_Document) delete m_Document;
}

bool FrLoadTabsCmd::Execute(){
    if(!this->GetMainController()) return false;
    if(m_FileName.isNull() || m_FileName.isEmpty()) return false;
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
                FrTabSettingsDocObj* tsDO = (m_tabSets.size() == 0) ? 
                    new FrTabSettingsDocObj(true) : new FrTabSettingsDocObj();

                if(LoadTabSettings(tabSetElem, tsDO)){
                    m_tabSets.push_back(tsDO);
                }
                else {
                    delete tsDO;

                    // Delete all tabs
                    TabSettingsVec::iterator it, itEnd(m_tabSets.end());
                    for(it = m_tabSets.begin(); it != itEnd; ++it){
                        delete (*it);
                    }
                    m_tabSets.clear();

                    // Show message to user
                    FrMainWindow* mv = this->GetMainController()->GetMainView();
                    QMessageBox::critical(mv, "Loading Tab Sets",
                                              "Error: File is broken!");
                    return false;
                }
            }
            node = node.nextSibling();
        }

        // Add all tabSets to document
        if(m_tabSets.size() > 0){
            FrMainDocument* mainDoc =
                 this->GetMainController()->GetMainDocument();

            // Remove all tab setting objects from main doc
            std::vector<FrDocumentObj*> tabSettings;
            mainDoc->GetObjectsByType(tabSettings, FrDocumentObj::TabSettings);
            std::vector<FrDocumentObj*>::iterator itr, itrEnd(tabSettings.end());
            for(itr = tabSettings.begin(); itr != itrEnd; ++itr){
                mainDoc->Remove( (*itr) );
            }

            // Add new settings
            TabSettingsVec::iterator it, itEnd(m_tabSets.end());
            for(it = m_tabSets.begin(); it != itEnd; ++it){
                // NOTE: Make first tab current
                (*it)->SetIsCurrent((it == m_tabSets.begin()));
                mainDoc->Add(*it);
            }

            // copy settings from current tab to view doc object
            FrViewDocObj* viewDO = mainDoc->GetCurrentViewObject();
            viewDO->CopySettingsFrom(mainDoc->GetCurrentTabSettings());
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

    // Read all attributes
    if(!elem.hasAttribute(FR_XML_NAME_ATTR)) return false;
    tabs->SetName(elem.attribute(FR_XML_NAME_ATTR));

    //if(!elem.hasAttribute(FR_XML_DESCRIPTION_ATTR)) return false;
    //tabs->SetDescription(elem.attribute(FR_XML_DESCRIPTION_ATTR));

    bool isActiveViewOk = false;
    if(!elem.hasAttribute(FR_XML_ACTIVEVIEW_ATTR)) return false;
    int activeView = elem.attribute(FR_XML_ACTIVEVIEW_ATTR).toInt(&isActiveViewOk);

    if(!isActiveViewOk) return false;
    switch(activeView){
        // NOTE: Fall throuhg first 3 cases 
        case SliceView:
        case MosaicView:
        case OrthoView:
            tabs->SetActiveView((Views)activeView);
            break;
        default: 
            return false;
            break;
    }

    // Read view settings
    bool hasSlice, hasMosaic, hasOrtho, hasLayeredImage;
    hasSlice = hasMosaic = hasOrtho = hasLayeredImage = false;

    QDomElement viewElem = elem.firstChildElement();
    while(!viewElem.isNull()){
        if(viewElem.tagName() == FR_XML_SLICEVIEW_ELEM){
            hasSlice = LoadSliceViewSettings(viewElem, tabs->GetSliceViewSettings());
            if(!hasSlice) return false;
        }
        else if(viewElem.tagName() == FR_XML_MOSAICVIEW_ELEM){
            hasMosaic = LoadMosaicViewSettings(viewElem, tabs->GetMosaicViewSettings());
            if(!hasMosaic) return false;
        }
        else if(viewElem.tagName() == FR_XML_ORTHOVIEW_ELEM){
            hasOrtho = LoadOrthoViewSettings(viewElem, tabs->GetOrthoViewSettings());
            if(!hasOrtho) return false;
        }
        else if(viewElem.tagName() == FR_XML_LIS_ELEM){
            hasLayeredImage = LoadLayeredImageSettings(viewElem, 
                              tabs->GetImageLayer(), tabs->GetLayers());
            if(!hasLayeredImage) return false;       
        }
        viewElem = viewElem.nextSiblingElement();
    }

    bool result = (hasSlice && hasMosaic && hasOrtho && hasLayeredImage);
//    if(result){
//        result = ValidateTabSettings(tabs);
//    }
    return result;
}

bool FrLoadTabsCmd::LoadSliceViewSettings(QDomElement& elem,  FrSliceViewSettings* vs){
    if(!elem.hasChildNodes()) return false;
    
    bool hasSliceNum, hasActiveLayer, hasCamera, hasLayeredImage;
    hasSliceNum = hasActiveLayer = hasCamera = hasLayeredImage = false;

    // Load settings
    QDomElement childElem = elem.firstChildElement();
    while(!childElem.isNull()){
        if(childElem.tagName() == FR_XML_SLICENUM_ELEM){
            // Load slice number
            if(!childElem.hasAttribute(FR_XML_VALUE_ATTR)) return false;
            vs->SliceNumber = childElem.attribute(FR_XML_VALUE_ATTR).toInt(&hasSliceNum);
            if(!hasSliceNum) return false;
        }
        else if(childElem.tagName() == FR_XML_LAYERNUM_ELEM){
            // Load active layer id
            if(!childElem.hasAttribute(FR_XML_VALUE_ATTR)) return false;
            vs->ActiveLayerID = childElem.attribute(FR_XML_VALUE_ATTR).toInt(&hasActiveLayer);
            if(!hasActiveLayer) return false;
            // NOTE: add this fix
            vs->ActiveLayerID = DEF_LAYER_ID;
        }
        else if(childElem.tagName() == FR_XML_CAMS_ELEM){
            // Load camera settings for slice view
            if(!childElem.hasAttribute(FR_XML_COUNT_ATTR)) return false;
            int count = childElem.attribute(FR_XML_COUNT_ATTR).toInt(&hasCamera);
            if(!hasCamera || count != 1) return false;

            QDomElement camElem = childElem.firstChildElement();
            if(camElem.isNull()) return false;

            hasCamera = LoadCameraSettings(camElem, &vs->CamSettings);
            if(!hasCamera) return false;
        }
        childElem = childElem.nextSiblingElement();
    }
    return (hasSliceNum && hasActiveLayer && hasCamera);
}

bool FrLoadTabsCmd::LoadMosaicViewSettings(QDomElement& elem, FrMosaicViewSettings* vs){
    if(!elem.hasChildNodes()) return false;
    
    bool hasActiveLayer, hasCamera, hasLayeredImage;
    hasActiveLayer = hasCamera = hasLayeredImage = false;

    // Load settings
    QDomElement childElem = elem.firstChildElement();
    while(!childElem.isNull()){
        if(childElem.tagName() == FR_XML_LAYERNUM_ELEM){
            // Load active layer id
            if(!childElem.hasAttribute(FR_XML_VALUE_ATTR)) return false;
            vs->ActiveLayerID = childElem.attribute(FR_XML_VALUE_ATTR).toInt(&hasActiveLayer);
            if(!hasActiveLayer) return false;
            // NOTE: add this fix
            vs->ActiveLayerID = DEF_LAYER_ID;
        }
        else if(childElem.tagName() == FR_XML_CAMS_ELEM){
            // Load camera settings for slice view
            if(!childElem.hasAttribute(FR_XML_COUNT_ATTR)) return false;
            int count = childElem.attribute(FR_XML_COUNT_ATTR).toInt(&hasCamera);
            if(!hasCamera || count != 1) return false;

            QDomElement camElem = childElem.firstChildElement();
            if(camElem.isNull()) return false;

            hasCamera = LoadCameraSettings(camElem, &vs->CamSettings);
            if(!hasCamera) return false;
        }
        childElem = childElem.nextSiblingElement();
    }
    return (hasActiveLayer && hasCamera);
}

bool FrLoadTabsCmd::LoadOrthoViewSettings(QDomElement& elem,  FrOrthoViewSettings* vs){
    if(!elem.hasChildNodes()) return false;
    
    bool hasSliceNum, hasActiveLayer, hasCamera, hasLayeredImage;
    hasSliceNum = hasActiveLayer = hasCamera = hasLayeredImage = false;

    // Load settings
    QDomElement childElem = elem.firstChildElement();
    while(!childElem.isNull()){
        if(childElem.tagName() == FR_XML_SLICENUM_ELEM){
            // Load slice number
            char* attrNames[] = {FR_XML_CORONAL_ATTR, FR_XML_SAGITAL_ATTR, FR_XML_AXIAL_ATTR };
            int* attributes[] = { &vs->SliceNumber[DEF_CORONAL], 
                &vs->SliceNumber[DEF_SAGITAL], 
                &vs->SliceNumber[DEF_AXIAL] };

            for(int i=0; i < 3; ++i){
                if(!childElem.hasAttribute(attrNames[i])) return false;
                (*(attributes[i])) = childElem.attribute(attrNames[i]).toInt(&hasSliceNum);
                if(!hasSliceNum) return false;
            }
        }
        else if(childElem.tagName() == FR_XML_LAYERNUM_ELEM){
            // Load active layer id
            if(!childElem.hasAttribute(FR_XML_VALUE_ATTR)) return false;
            vs->ActiveLayerID = childElem.attribute(FR_XML_VALUE_ATTR).toInt(&hasActiveLayer);
            if(!hasActiveLayer) return false;
            // NOTE: add this fix
            vs->ActiveLayerID = DEF_LAYER_ID;
        }
        else if(childElem.tagName() == FR_XML_CAMS_ELEM){
            // Load camera settings for slice view
            if(!childElem.hasAttribute(FR_XML_COUNT_ATTR)) return false;
            int camCount = childElem.attribute(FR_XML_COUNT_ATTR).toInt(&hasCamera);
            if(!hasCamera || camCount != 3) return false;

            // Load three cameras
            int loadedCamCount = 0;
            QDomElement camElem = childElem.firstChildElement();
            while(!camElem.isNull()){
                int camIdx = loadedCamCount % 3;
                if( LoadCameraSettings(camElem, &vs->CamSettings[camIdx]) ){
                    loadedCamCount += 1;
                }                
                camElem = camElem.nextSiblingElement();
            }

            hasCamera = (loadedCamCount == camCount);
            if(!hasCamera) return false;
        }
        childElem = childElem.nextSiblingElement();
    }
    return (hasSliceNum && hasActiveLayer && hasCamera);
}

bool FrLoadTabsCmd::LoadCameraSettings(QDomElement& elem, FrCameraSettings* cs){
    if(elem.tagName() != FR_XML_CAM_ELEM) return false;
    
    // read scale
    bool hasScale, hasUp, hasPos, hasFP;
    hasScale = hasUp = hasPos = hasFP = false;
    
    QDomElement childElem = elem.firstChildElement();
    while(!childElem.isNull()){
        if(childElem.tagName() == FR_XML_SCALE_ELEM){
            // Read scale
            if(!childElem.hasAttribute(FR_XML_VALUE_ATTR)) return false;
            cs->Scale = childElem.attribute(FR_XML_VALUE_ATTR).toDouble(&hasScale);
            if(!hasScale) return false;
        }
        else if(childElem.tagName() == FR_XML_VIEWUP_ELEM){
            hasUp = LoadAttrValuesXYZ(childElem, cs->ViewUp);
            if(!hasUp) return false;
        }
        else if(childElem.tagName() == FR_XML_POS_ELEM){
            hasPos = LoadAttrValuesXYZ(childElem, cs->Position);
            if(!hasPos) return false;
        }
        else if(childElem.tagName() == FR_XML_FOCUS_ELEM){
            hasFP = LoadAttrValuesXYZ(childElem, cs->FocalPoint);
            if(!hasFP) return false;
        }
        childElem = childElem.nextSiblingElement();
    }   
    return (hasScale && hasUp && hasPos && hasFP);
}

bool FrLoadTabsCmd::LoadLayeredImageSettings(QDomElement& elem, 
                                             FrImageLayerSettings* mlSets, 
                                             std::vector<FrLayerSettings*>& olSets){
    
    QDomElement imageElem = elem.firstChildElement();
    if(imageElem.tagName() != FR_XML_IMG_ELEM) return false;

    //Load main settings
    bool isValid = false;
    // ID
    //if(!elem.hasAttribute(FR_XML_ID_ATTR)) return false;
    //mlSets->ID = elem.attribute(FR_XML_ID_ATTR).toInt(&isValid);
    //if(!isValid) return false;
    // NOTE: ID is always 0 for main layer of the image
//    mlSets->ID = DEFAULT_LAYER_ID;

    if(!imageElem.hasAttribute(FR_XML_NAME_ATTR)) return false;
    mlSets->Name = imageElem.attribute(FR_XML_NAME_ATTR);
    if(mlSets->Name != QString(DEF_LAYER_NAME)) return false;

    // Opacity    
    if(!imageElem.hasAttribute(FR_XML_OPACITY_ATTR)) return false;
    mlSets->Opacity = imageElem.attribute(FR_XML_OPACITY_ATTR).toDouble(&isValid);
    if(isValid) mlSets->Opacity = ClampValue(mlSets->Opacity, 0.0, 1.0);
    else return false;
    // Visibility
    if(!imageElem.hasAttribute(FR_XML_VISIBLE_ATTR)) return false;
    int intValue = imageElem.attribute(FR_XML_VISIBLE_ATTR).toInt(&isValid);
    if(!isValid || (intValue != 0 && intValue != 1)) return false;
    mlSets->Visibility = (intValue != 0);

    bool hasTbc, hasLayers;
    hasTbc = hasLayers = false;
    QDomElement childElem = imageElem.firstChildElement();
    while(!childElem.isNull()){
        if(childElem.tagName() == FR_XML_TBCSETS_ELEM){
            // Load TBC settings
            hasTbc = LoadTbcSettings(childElem, &mlSets->TbcSettings);
            if(!hasTbc) return false;
        }
        else if(childElem.tagName() == FR_XML_LAYERS_ELEM){
            // Load layers
            hasLayers = LoadLayersSettings(childElem, olSets);
        }
        childElem = childElem.nextSiblingElement();
    }
    return (hasTbc && hasLayers);
}

bool FrLoadTabsCmd::LoadTbcSettings(QDomElement& elem, FrTbcSettings* ts){
    if(elem.tagName() != FR_XML_TBCSETS_ELEM) return false;

    bool hasThreshold, hasBrightness, hasContrast;
    hasThreshold = hasBrightness = hasContrast = false;

    QDomElement childElem = elem.firstChildElement();
    while(!childElem.isNull()){
        if(childElem.tagName() == FR_XML_THRESH_ELEM){
            if(!childElem.hasAttribute(FR_XML_VALUE_ATTR)) return false;
            ts->Threshold = childElem.attribute(FR_XML_VALUE_ATTR).toDouble(&hasThreshold);

            if(hasThreshold) ts->Threshold = ClampValue(ts->Threshold, 0.0, 1.0);
            else return false;
        }
        else if(childElem.tagName() == FR_XML_BRIGHT_ELEM){
            if(!childElem.hasAttribute(FR_XML_VALUE_ATTR)) return false;
            ts->Brightness = childElem.attribute(FR_XML_VALUE_ATTR).toDouble(&hasBrightness);

            if(hasBrightness) ts->Brightness = ClampValue(ts->Brightness, -1.0, 1.0);
            else return false;
        }
        else if(childElem.tagName() == FR_XML_CONTRAST_ELEM){
            if(!childElem.hasAttribute(FR_XML_VALUE_ATTR)) return false;
            ts->Contrast = childElem.attribute(FR_XML_VALUE_ATTR).toDouble(&hasContrast);

            if(hasContrast) ts->Contrast = ClampValue(ts->Contrast, -1.0, 1.0);
            else return false;
        }
        childElem = childElem.nextSiblingElement();
    }

    return (hasThreshold && hasBrightness && hasContrast);
}

bool FrLoadTabsCmd::LoadLayersSettings(QDomElement& elem, 
                                       std::vector<FrLayerSettings*>& olSets){
    if(elem.tagName() != FR_XML_LAYERS_ELEM) return false;
    if(!elem.hasAttribute(FR_XML_COUNT_ATTR)) return false;

    // Get count from xml
    bool isValid = false;
    int readCount = elem.attribute(FR_XML_COUNT_ATTR).toInt(&isValid);
    if(!isValid) return false;

    QDomElement childElem = elem.firstChildElement();
    while(!childElem.isNull()){
        if(childElem.tagName() == FR_XML_LAYER_ELEM){
            // read layer type first
            // TODO: implement
            FrLayerSettings::LTypes ltype = (FrLayerSettings::LTypes)childElem.attribute(FR_XML_TYPE_ATTR).toInt(&isValid);
            if(!isValid) return false;

            if(ltype == FrLayerSettings::LRoi){
                FrRoiLayerSettings* layerSettings = new FrRoiLayerSettings();

                if(LoadLayerSettings(childElem, layerSettings)){
                    olSets.push_back(layerSettings);
                }
                else {
                    delete layerSettings;
                    return false;
                }
            }
            else{
                FrColormapLayerSettings* layerSettings = new FrColormapLayerSettings();

                if(LoadLayerSettings(childElem, layerSettings)){
                    olSets.push_back(layerSettings);
                }
                else {
                    delete layerSettings;
                    return false;
                }
            }
        }
        childElem = childElem.nextSiblingElement();
    }

    // get and read count must be the same
    return (olSets.size() == readCount);
}

bool FrLoadTabsCmd::LoadLayerSettings(QDomElement& elem, FrLayerSettings* ls){
    if(elem.tagName() != FR_XML_LAYER_ELEM) return false;

    bool isValid = false;
    // NOTE for now ignore id attribute
//    ls->ID = BAD_LAYER_ID;    

    // Name
    if(!elem.hasAttribute(FR_XML_NAME_ATTR)) return false;
    ls->Name = elem.attribute(FR_XML_NAME_ATTR);

    // Opacity    
    if(!elem.hasAttribute(FR_XML_OPACITY_ATTR)) return false;
    ls->Opacity = elem.attribute(FR_XML_OPACITY_ATTR).toDouble(&isValid);
    if(isValid) ls->Opacity = ClampValue(ls->Opacity, 0.0, 1.0);
    else return false;
    // Visibility
    if(!elem.hasAttribute(FR_XML_VISIBLE_ATTR)) return false;
    int intValue = elem.attribute(FR_XML_VISIBLE_ATTR).toInt(&isValid);
    if(!isValid || (intValue != 0 && intValue != 1)) return false;
    ls->Visibility = (intValue != 0);

    FrLayerSettings::LTypes ltype = ls->GetType();
    bool hasRange, hasCm, hasTbc;

    switch (ltype){
        case FrLayerSettings::LRoi:
            return true;
            // do nothing
            break;
        case FrLayerSettings::LColormap:
            FrColormapLayerSettings* cmlSets = (FrColormapLayerSettings*)ls;

            // Load other
            hasRange = hasCm = hasTbc = false;

            QDomElement childElem = elem.firstChildElement();
            while(!childElem.isNull()){
                if(childElem.tagName() == FR_XML_PXRANGE_ELEM){
                    if(!childElem.hasAttribute(FR_XML_MAXVAL_ATTR) ||
                       !childElem.hasAttribute(FR_XML_MINVAL_ATTR)) return false;

                    cmlSets->ColormapSettings.MinValue = 
                        childElem.attribute(FR_XML_MINVAL_ATTR).toInt(&hasRange);
                    if(!hasRange) return false;

                    cmlSets->ColormapSettings.MaxValue = 
                        childElem.attribute(FR_XML_MAXVAL_ATTR).toInt(&hasRange);
                    if(!hasRange) return false;

                }
                else if(childElem.tagName() == FR_XML_CMAP_ELEM){
                    if(!childElem.hasAttribute(FR_XML_MIDVALUE_ATTR) ||
                       !childElem.hasAttribute(FR_XML_THRESH_ATTR)   ||
                       !childElem.hasAttribute(FR_XML_TYPE_ATTR)   ||
                       !childElem.hasAttribute(FR_XML_COLOR_ATTR)) return false;
                    // Get colormap type
                    hasCm = false;
                    QString value = childElem.attribute(FR_XML_TYPE_ATTR);
                    if(value == FR_CMTYPE_SINGLE){
                        cmlSets->ColormapSettings.Type = FrColormapSettings::SingleColor;
                        hasCm = true;
                    }
                    else if(value == FR_CMTYPE_MULTI){
                        cmlSets->ColormapSettings.Type = FrColormapSettings::MultiColor;
                        hasCm = true;
                    }
                    if(!hasCm) return false;

                    // Get color
                    value = childElem.attribute(FR_XML_COLOR_ATTR);
                    if(value.length() != 7) return false;
                    int r = value.mid(1,2).toInt(&hasCm, 16);
                    if(!hasCm) return false;
                    int g = value.mid(3,2).toInt(&hasCm, 16);
                    if(!hasCm) return false;
                    int b = value.mid(5,2).toInt(&hasCm, 16);
                    if(!hasCm) return false;
                    cmlSets->ColormapSettings.Color.setRgb(r, g, b);

                    // Get mid value
                    cmlSets->ColormapSettings.MidValue = 
                    childElem.attribute(FR_XML_MIDVALUE_ATTR).toInt(&hasCm);
                    // Get threshold	(range of non significant values)
                    cmlSets->ColormapSettings.Threshold = 
                    childElem.attribute(FR_XML_THRESH_ATTR).toInt(&hasCm);
                    if(!hasCm) return false;
                }
                else if(childElem.tagName() == FR_XML_TBCSETS_ELEM){
                    hasTbc = LoadTbcSettings(childElem, &cmlSets->TbcSettings);
                    if(!hasTbc) return false;
                }
                childElem = childElem.nextSiblingElement();
            }
            break;
    }

    return (hasRange && hasCm && hasTbc);
}

bool FrLoadTabsCmd::LoadAttrValuesXYZ(QDomElement& elem, double* vec){
    // Check input
    bool hasRequiredAttributes = elem.hasAttribute(FR_XML_X_ATTR) &&
                                 elem.hasAttribute(FR_XML_Y_ATTR) && 
                                 elem.hasAttribute(FR_XML_Z_ATTR);
    if(!hasRequiredAttributes) return false;

    // load values
    bool isOk = false;
    vec[0] = elem.attribute(FR_XML_X_ATTR).toDouble(&isOk);
    if(!isOk) return false;

    vec[1] = elem.attribute(FR_XML_Y_ATTR).toDouble(&isOk);
    if(!isOk) return false;

    vec[2] = elem.attribute(FR_XML_Z_ATTR).toDouble(&isOk);
    return isOk;
}
//
//bool FrLoadTabsCmd::ValidateTabSettings(FrTabSettingsDocObj* tabs){
//    // All views have to have the same layers
//    LayersCollection* layers;
//    layers = &tabs->GetLayers();
//    
//    // Check for the same layers
//    bool result = true;
//    LayersCollection* testLayers = &tabs->GetSliceViewSettings()->OtherLayers;
//    for(int i=0; i < 4; ++i){
//        if(layers[i]->size() != testLayers->size()){
//            result = false;
//            break;
//        }
//
//        // Check some layers params and also fix IDS
////        int layerID = 1;
//        for(int layerNum = 0; layerNum < layers[i]->size(); ++layerNum){
//            FrLayerSettings* ls0 = layers[i]->operator [](layerNum);
//            FrLayerSettings* ls1 = testLayers->operator [](layerNum);
//
////            ls0->ID = layerID;
////            ls1->ID = layerID;
////            layerID++;
//
//            result = (ls0->Opacity == ls1->Opacity) &&
//                (ls0->Visibility == ls1->Visibility) &&
//                (ls0->ColormapSettings.Type == ls1->ColormapSettings.Type) &&
//                (ls0->ColormapSettings.MinValue == ls1->ColormapSettings.MinValue) &&
//                (ls0->ColormapSettings.MaxValue == ls1->ColormapSettings.MaxValue) &&
//                (ls0->ColormapSettings.MidValue == ls1->ColormapSettings.MidValue) &&
//                (ls0->ColormapSettings.Threshold == ls1->ColormapSettings.Threshold) &&
//                (ls0->ColormapSettings.Color     == ls1->ColormapSettings.Color);
//
//            if(!result) break;
//        }
//    }
//    return result;
//}

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
