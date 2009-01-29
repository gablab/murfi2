#include "FrLoadGraphTabSettingsCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrGraphTabDocObj.h"
#include "FrPointsDocObj.h"
#include "FrXmlDefs.h"
#include "FrUtils.h"
#include "FrImageDocObj.h"
#include "RtMRIImage.h"

// Qt stuff
#include "Qt/qfile.h"
#include "Qt/qtextstream.h"
#include "QtXml/qdom.h"
#include "Qt/qmessagebox.h"



FrLoadGraphTabSettingsCmd::FrLoadGraphTabSettingsCmd()
: m_FileName(""), m_Document(0) {
    m_Document = new QDomDocument();
}

FrLoadGraphTabSettingsCmd::~FrLoadGraphTabSettingsCmd(){
    if(m_Document) delete m_Document;
}

bool FrLoadGraphTabSettingsCmd::Execute(){
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
        if(!isVersionOk || version != FR_GRAPH_TAB_SET_VERSION) return false;

        // if root found and version is ok then load settings
        QDomNode node = root.firstChild();
        while(!node.isNull()){
            QDomElement tabSetElem = node.toElement();
            if(!tabSetElem.isNull() && 
                tabSetElem.tagName() == FR_XML_GRAPHTABSETTINGS_ELEM){
                // Create tab settings
                FrGraphTabDocObj* tsDO = (m_tabSets.size() == 0) ? 
                    new FrGraphTabDocObj(true) : new FrGraphTabDocObj(false);

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
            mainDoc->GetObjectsByType(tabSettings, FrDocumentObj::GraphTab);
            std::vector<FrDocumentObj*>::iterator itr, itrEnd(tabSettings.end());
            for(itr = tabSettings.begin(); itr != itrEnd; ++itr){
                mainDoc->Remove( (*itr) );
            }

            // remove points doc obj
            FrPointsDocObj* pointsDO = mainDoc->GetCurrentPointsObject();
            if (pointsDO){
                mainDoc->Remove(pointsDO);
            }

            // Add new settings
            TabSettingsVec::iterator it, itEnd(m_tabSets.end());
            for(it = m_tabSets.begin(); it != itEnd; ++it){
                // NOTE: Make first tab current
                (*it)->SetIsCurrent((it == m_tabSets.begin()));
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

bool FrLoadGraphTabSettingsCmd::LoadTabSettings(QDomElement& elem, FrGraphTabDocObj* tabs){
    if(!elem.hasChildNodes()) return false;

    // Read all attributes
    if(!elem.hasAttribute(FR_XML_NAME_ATTR)) return false;
    tabs->SetName(elem.attribute(FR_XML_NAME_ATTR));

    //if(!elem.hasAttribute(FR_XML_DESCRIPTION_ATTR)) return false;
    //tabs->SetDescription(elem.attribute(FR_XML_DESCRIPTION_ATTR));

    bool hasPoints, hasGraphs;
    hasPoints = hasGraphs = false;

    QDomElement viewElem = elem.firstChildElement();
    while(!viewElem.isNull()){
        if(viewElem.tagName() == FR_XML_POINTS_ELEM){
            hasPoints = LoadPoints(viewElem, tabs->GetPoints());
            //if(!hasPoints) return false;
        }
        else if(viewElem.tagName() == FR_XML_GRAPHS_ELEM){
            hasGraphs = LoadGraphs(viewElem, tabs->GetGraphs());
            //if(!hasGraphs) return false;
        }
        viewElem = viewElem.nextSiblingElement();
    }

    bool result = true; 
    if (!hasPoints && hasGraphs)
        result = false;             // graphs can't exist w/o point selected

    return result;
}

bool FrLoadGraphTabSettingsCmd::LoadPoints(QDomElement& elem, FrPointsDocObj* pointsDo){
    bool result = false;

    QDomElement pointElem = elem.firstChildElement();
    while(!pointElem.isNull()){
        if(pointElem.tagName() == FR_XML_POINT_ELEM){
            // load point
            int x, y, z;
            QColor color;

            if(!pointElem.hasAttribute(FR_XML_X_ATTR)) return false;
            x = pointElem.attribute(FR_XML_X_ATTR).toInt(&result);
            if(!result) return false;
            if(!pointElem.hasAttribute(FR_XML_Y_ATTR)) return false;
            y = pointElem.attribute(FR_XML_Y_ATTR).toInt(&result);
            if(!result) return false;
            if(!pointElem.hasAttribute(FR_XML_Z_ATTR)) return false;
            z = pointElem.attribute(FR_XML_Z_ATTR).toInt(&result);
            if(!result) return false;

            // Get color
            QString value = pointElem.attribute(FR_XML_COLOR_ATTR);
            if(value.length() != 7) return false;
            int r = value.mid(1,2).toInt(&result, 16);
            if(!result) return false;
            int g = value.mid(3,2).toInt(&result, 16);
            if(!result) return false;
            int b = value.mid(5,2).toInt(&result, 16);
            if(!result) return false;
            color.setRgb(r, g, b);

            FrPoint* point = new FrPoint(x, y, z, color);
            pointsDo->AddPoint(point);
        }

        pointElem = pointElem.nextSiblingElement();
    }

    return result;
}

bool FrLoadGraphTabSettingsCmd::LoadGraphs(QDomElement& elem, std::vector<FrGraphDocObj*>& graphs){
    bool result = false;

    graphs.clear();

    QDomElement graphElem = elem.firstChildElement();
    while(!graphElem.isNull()){
        if(graphElem.tagName() == FR_XML_GRAPH_ELEM){
            // read timeseria       
            if(!graphElem.hasAttribute(FR_XML_TIMESERIA_ATTR)) return false;
            int timeseria = graphElem.attribute(FR_XML_TIMESERIA_ATTR).toInt(&result);
            if(!result) return false;
            FrGraphSettings* gs;

            QDomElement gsElem = graphElem.firstChildElement();
            if(gsElem.tagName() == FR_XML_GRAPH_SETTINGS_ELEM){
                // read type
                if(!gsElem.hasAttribute(FR_XML_GRAPHTYPE_ATTR)) return false;
                FrGraphSettings::GraphTypes type = (FrGraphSettings::GraphTypes)
                    gsElem.attribute(FR_XML_GRAPHTYPE_ATTR).toInt(&result);
                if(!result) return false;

                // read common parameters
                // Get color
                QColor color;
                QString value = gsElem.attribute(FR_XML_COLOR_ATTR);
                if(value.length() != 7) return false;
                int r = value.mid(1,2).toInt(&result, 16);
                if(!result) return false;
                int g = value.mid(3,2).toInt(&result, 16);
                if(!result) return false;
                int b = value.mid(5,2).toInt(&result, 16);
                if(!result) return false;
                color.setRgb(r, g, b);

                // get draw method
                if(!gsElem.hasAttribute(FR_XML_DRAWMETHOD_ATTR)) return false;
                FrGraphSettings::DrawMethods method = (FrGraphSettings::DrawMethods)
                    gsElem.attribute(FR_XML_DRAWMETHOD_ATTR).toInt(&result);
                if(!result) return false;
                
                // get visibility
                if(!gsElem.hasAttribute(FR_XML_VISIBLE_ATTR)) return false;
                int intValue = gsElem.attribute(FR_XML_VISIBLE_ATTR).toInt(&result);
                if(!result || (intValue != 0 && intValue != 1)) return false;
                bool visible = (intValue != 0);

                // get name
                /*if(!gsElem.hasAttribute(FR_XML_NAME_ATTR)) return false;
                QString name = gsElem.attribute(FR_XML_NAME_ATTR).toInt(&result);
                if(!result) return false;*/

                int x, y, z;
                QDomElement voxelElem;

                switch (type){
                    case FrGraphSettings::GT_Intencity:
                        gs = new FrIntencityGraphSettings();

                        // read voxel
                        voxelElem = gsElem.firstChildElement();
                        
                        if(!voxelElem.hasAttribute(FR_XML_X_ATTR)) return false;
                        x = voxelElem.attribute(FR_XML_X_ATTR).toInt(&result);
                        if(!result) return false;
                        if(!voxelElem.hasAttribute(FR_XML_Y_ATTR)) return false;
                        y = voxelElem.attribute(FR_XML_Y_ATTR).toInt(&result);
                        if(!result) return false;
                        if(!voxelElem.hasAttribute(FR_XML_Z_ATTR)) return false;
                        z = voxelElem.attribute(FR_XML_Z_ATTR).toInt(&result);
                        if(!result) return false;
                       
                        ((FrIntencityGraphSettings*)gs)->I = x;
                        ((FrIntencityGraphSettings*)gs)->J = y;
                        ((FrIntencityGraphSettings*)gs)->K = z;
                        break;
                    case FrGraphSettings::GT_Movements:
                        //gs = new FrMovementsGraphSettings();
                        break;
                    case FrGraphSettings::GT_Stimulus:
                        //gs = new FrStimulusGraphSettings();
                        break;
                    case FrGraphSettings::GT_RoiMean:
                        //gs = new FrRoiMeanGraphSettings();
                        break;
                    case FrGraphSettings::GT_RoiStd:
                        //gs = new FrRoiStdGraphSettings();
                        break;
                }

                //gs->Name = name;
                gs->Color = color;
                gs->Visibility = visible;
                gs->DrawMethod = method;

                // create new graph doc obj and add it to graphs
                FrGraphDocObj* graphDO = new FrGraphDocObj(type);
                graphDO->SetTimeSeria(timeseria);
                graphDO->SetSettings(gs);
                graphs.push_back(graphDO);
            }
            else
                return false;
        }

        graphElem = graphElem.nextSiblingElement();
    }

    return result;
}


/*
bool FrLoadGraphTabSettingsCmd::LoadLayeredImageSettings(QDomElement& elem, 
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
*/
//bool FrLoadGraphTabSettingsCmd::LoadTbcSettings(QDomElement& elem, FrTbcSettings* ts){
//    if(elem.tagName() != FR_XML_TBCSETS_ELEM) return false;
//
//    bool hasThreshold, hasBrightness, hasContrast;
//    hasThreshold = hasBrightness = hasContrast = false;
//
//    QDomElement childElem = elem.firstChildElement();
//    while(!childElem.isNull()){
//        if(childElem.tagName() == FR_XML_THRESH_ELEM){
//            if(!childElem.hasAttribute(FR_XML_VALUE_ATTR)) return false;
//            ts->Threshold = childElem.attribute(FR_XML_VALUE_ATTR).toDouble(&hasThreshold);
//
//            if(hasThreshold) ts->Threshold = ClampValue(ts->Threshold, 0.0, 1.0);
//            else return false;
//        }
//        else if(childElem.tagName() == FR_XML_BRIGHT_ELEM){
//            if(!childElem.hasAttribute(FR_XML_VALUE_ATTR)) return false;
//            ts->Brightness = childElem.attribute(FR_XML_VALUE_ATTR).toDouble(&hasBrightness);
//
//            if(hasBrightness) ts->Brightness = ClampValue(ts->Brightness, -1.0, 1.0);
//            else return false;
//        }
//        else if(childElem.tagName() == FR_XML_CONTRAST_ELEM){
//            if(!childElem.hasAttribute(FR_XML_VALUE_ATTR)) return false;
//            ts->Contrast = childElem.attribute(FR_XML_VALUE_ATTR).toDouble(&hasContrast);
//
//            if(hasContrast) ts->Contrast = ClampValue(ts->Contrast, -1.0, 1.0);
//            else return false;
//        }
//        childElem = childElem.nextSiblingElement();
//    }
//
//    return (hasThreshold && hasBrightness && hasContrast);
//}
/*
bool FrLoadGraphTabSettingsCmd::LoadLayersSettings(QDomElement& elem, 
                                       std::vector<FrLayerSettings*>& olSets){
    //if(elem.tagName() != FR_XML_LAYERS_ELEM) return false;
    //if(!elem.hasAttribute(FR_XML_COUNT_ATTR)) return false;

    //// Get count from xml
    //bool isValid = false;
    //int readCount = elem.attribute(FR_XML_COUNT_ATTR).toInt(&isValid);
    //if(!isValid) return false;

    //QDomElement childElem = elem.firstChildElement();
    //while(!childElem.isNull()){
    //    if(childElem.tagName() == FR_XML_LAYER_ELEM){
    //        // read layer type first
    //        // TODO: implement
    //        FrLayerSettings::LTypes ltype = (FrLayerSettings::LTypes)childElem.attribute(FR_XML_TYPE_ATTR).toInt(&isValid);
    //        if(!isValid) return false;

    //        if(ltype == FrLayerSettings::LRoi){
    //            FrRoiLayerSettings* layerSettings = new FrRoiLayerSettings();

    //            if(LoadLayerSettings(childElem, layerSettings)){
    //                olSets.push_back(layerSettings);
    //            }
    //            else {
    //                delete layerSettings;
    //                return false;
    //            }
    //        }
    //        else{
    //            FrColormapLayerSettings* layerSettings = new FrColormapLayerSettings();

    //            if(LoadLayerSettings(childElem, layerSettings)){
    //                olSets.push_back(layerSettings);
    //            }
    //            else {
    //                delete layerSettings;
    //                return false;
    //            }
    //        }
    //    }
    //    childElem = childElem.nextSiblingElement();
    //}

    //// get and read count must be the same
    //return (olSets.size() == readCount);
    return false;
}

bool FrLoadGraphTabSettingsCmd::LoadLayerSettings(QDomElement& elem, FrLayerSettings* ls){
//    if(elem.tagName() != FR_XML_LAYER_ELEM) return false;
//
//    bool isValid = false;
//    // NOTE for now ignore id attribute
////    ls->ID = BAD_LAYER_ID;    
//
//    // Name
//    if(!elem.hasAttribute(FR_XML_NAME_ATTR)) return false;
//    ls->Name = elem.attribute(FR_XML_NAME_ATTR);
//
//    // Opacity    
//    if(!elem.hasAttribute(FR_XML_OPACITY_ATTR)) return false;
//    ls->Opacity = elem.attribute(FR_XML_OPACITY_ATTR).toDouble(&isValid);
//    if(isValid) ls->Opacity = ClampValue(ls->Opacity, 0.0, 1.0);
//    else return false;
//    // Visibility
//    if(!elem.hasAttribute(FR_XML_VISIBLE_ATTR)) return false;
//    int intValue = elem.attribute(FR_XML_VISIBLE_ATTR).toInt(&isValid);
//    if(!isValid || (intValue != 0 && intValue != 1)) return false;
//    ls->Visibility = (intValue != 0);
//
//    FrLayerSettings::LTypes ltype = ls->GetType();
//    bool hasRange, hasCm, hasTbc;
//
//    switch (ltype){
//        case FrLayerSettings::LRoi:
//            return true;
//            // do nothing
//            break;
//        case FrLayerSettings::LColormap:
//            FrColormapLayerSettings* cmlSets = (FrColormapLayerSettings*)ls;
//
//            // Load other
//            hasRange = hasCm = hasTbc = false;
//
//            QDomElement childElem = elem.firstChildElement();
//            while(!childElem.isNull()){
//                if(childElem.tagName() == FR_XML_PXRANGE_ELEM){
//                    if(!childElem.hasAttribute(FR_XML_MAXVAL_ATTR) ||
//                       !childElem.hasAttribute(FR_XML_MINVAL_ATTR)) return false;
//
//                    cmlSets->ColormapSettings.MinValue = 
//                        childElem.attribute(FR_XML_MINVAL_ATTR).toInt(&hasRange);
//                    if(!hasRange) return false;
//
//                    cmlSets->ColormapSettings.MaxValue = 
//                        childElem.attribute(FR_XML_MAXVAL_ATTR).toInt(&hasRange);
//                    if(!hasRange) return false;
//
//                }
//                else if(childElem.tagName() == FR_XML_CMAP_ELEM){
//                    if(!childElem.hasAttribute(FR_XML_MIDVALUE_ATTR) ||
//                       !childElem.hasAttribute(FR_XML_THRESH_ATTR)   ||
//                       !childElem.hasAttribute(FR_XML_TYPE_ATTR)   ||
//                       !childElem.hasAttribute(FR_XML_COLOR_ATTR)) return false;
//                    // Get colormap type
//                    hasCm = false;
//                    QString value = childElem.attribute(FR_XML_TYPE_ATTR);
//                    if(value == FR_CMTYPE_SINGLE){
//                        cmlSets->ColormapSettings.Type = FrColormapSettings::SingleColor;
//                        hasCm = true;
//                    }
//                    else if(value == FR_CMTYPE_MULTI){
//                        cmlSets->ColormapSettings.Type = FrColormapSettings::MultiColor;
//                        hasCm = true;
//                    }
//                    if(!hasCm) return false;
//
//                    // Get color
//                    value = childElem.attribute(FR_XML_COLOR_ATTR);
//                    if(value.length() != 7) return false;
//                    int r = value.mid(1,2).toInt(&hasCm, 16);
//                    if(!hasCm) return false;
//                    int g = value.mid(3,2).toInt(&hasCm, 16);
//                    if(!hasCm) return false;
//                    int b = value.mid(5,2).toInt(&hasCm, 16);
//                    if(!hasCm) return false;
//                    cmlSets->ColormapSettings.Color.setRgb(r, g, b);
//
//                    // Get mid value
//                    cmlSets->ColormapSettings.MidValue = 
//                    childElem.attribute(FR_XML_MIDVALUE_ATTR).toInt(&hasCm);
//                    // Get threshold	(range of non significant values)
//                    cmlSets->ColormapSettings.Threshold = 
//                    childElem.attribute(FR_XML_THRESH_ATTR).toInt(&hasCm);
//                    if(!hasCm) return false;
//                }
//                else if(childElem.tagName() == FR_XML_TBCSETS_ELEM){
//                    hasTbc = LoadTbcSettings(childElem, &cmlSets->TbcSettings);
//                    if(!hasTbc) return false;
//                }
//                childElem = childElem.nextSiblingElement();
//            }
//            break;
//    }
//
//    return (hasRange && hasCm && hasTbc);
    return false;
}
*/
bool FrLoadGraphTabSettingsCmd::LoadAttrValuesXYZ(QDomElement& elem, double* vec){
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

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrLoadGraphTabSettingsCmd::CanUndo(){
    return false;
}

bool FrLoadGraphTabSettingsCmd::Undo(){
    return false;
}

bool FrLoadGraphTabSettingsCmd::Redo(){
    return false;
}
