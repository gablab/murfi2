#include "FrSaveGraphTabSettingsCmd.h"
#include "FrMainDocument.h"
#include "FrGraphTabDocObj.h"
#include "FrXmlDefs.h"
#include "FrPointsDocObj.h"

// Qt stuff
#include "Qt/qfile.h"
#include "Qt/qtextstream.h"
#include "QtXml/qdom.h"



// Some defines
#define DEF_NODE_INDENT 4
#define GET_HEX_COLOR_STRING(cl) ((cl == 0) ? QString("00") : QString().setNum(cl, 16))

FrSaveGraphTabSettingsCmd::FrSaveGraphTabSettingsCmd()
: m_FileName(""), m_Document(0) {
    m_Document = new QDomDocument();
}

FrSaveGraphTabSettingsCmd::~FrSaveGraphTabSettingsCmd(){
    if(m_Document) delete m_Document;
}

bool FrSaveGraphTabSettingsCmd::Execute(){
    if(!this->GetMainController()) return false;

    std::vector<FrDocumentObj*> tabSettings;
    FrMainDocument* mainDoc = this->GetMainController()->GetMainDocument();
    mainDoc->GetObjectsByType(tabSettings, FrDocumentObj::GraphTab);
    
    bool result = false;
    if(m_Document && !(m_FileName.isNull() || m_FileName.isEmpty())){
    
        // Add declaration and root element
        m_Document->clear();
        QDomProcessingInstruction declaration = 
            m_Document->createProcessingInstruction("xml", FR_XML_DECLARATION);

        QDomElement root = m_Document->createElement(FR_XML_GRAPH_ROOT_ELEM);
        root.setAttribute(FR_XML_VERSION_ATTR, FR_GRAPH_TAB_SET_VERSION);

        m_Document->appendChild(declaration);
        m_Document->appendChild(root);
        
        // Write to file
        QFile outFile(m_FileName);
        if (outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {

            // Add all settings
            for(int i = 0; i < tabSettings.size(); ++i){
                FrGraphTabDocObj* tsDO = (FrGraphTabDocObj*) tabSettings[i];
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

bool FrSaveGraphTabSettingsCmd::SaveTabSettings(QDomElement& root, FrGraphTabDocObj* tabs){
    if(!m_Document) return false;
    
    QDomElement tabSetsElem = m_Document->createElement(FR_XML_GRAPHTABSETTINGS_ELEM);
    tabSetsElem.setAttribute(FR_XML_NAME_ATTR, tabs->GetName());
    //tabSetsElem.setAttribute(FR_XML_DESCRIPTION_ATTR, tabs->GetDescription());
    root.appendChild(tabSetsElem);
    
    // save points doc obj
    QDomElement pointsElem = m_Document->createElement(FR_XML_POINTS_ELEM);
    FrPointsDocObj* pDO = tabs->GetPoints();
    // save points set
    this->SavePoints(pointsElem,  pDO);
    tabSetsElem.appendChild(pointsElem);

    // save graphs
    QDomElement graphsElem = m_Document->createElement(FR_XML_GRAPHS_ELEM);
    SaveGraphs(graphsElem, tabs->GetGraphs());
    tabSetsElem.appendChild(graphsElem);

    return true;
}

void FrSaveGraphTabSettingsCmd::SavePoints(QDomElement& parent, FrPointsDocObj* points){
    std::vector<FrPoint*> m_Points = points->GetPoints();
 
    for(int i = 0; i < m_Points.size(); ++i){
        FrPoint* p = (FrPoint*) m_Points[i];
            
        QDomElement pointElem = m_Document->createElement(FR_XML_POINT_ELEM);
        pointElem.setAttribute(FR_XML_X_ATTR, p->x);
        pointElem.setAttribute(FR_XML_Y_ATTR, p->y);
        pointElem.setAttribute(FR_XML_Z_ATTR, p->z);

        QColor color = p->color;
        QString colorValue = "#";
        colorValue += GET_HEX_COLOR_STRING(color.red());
        colorValue += GET_HEX_COLOR_STRING(color.green());
        colorValue += GET_HEX_COLOR_STRING(color.blue());

        pointElem.setAttribute(FR_XML_COLOR_ATTR, colorValue);
        parent.appendChild(pointElem);
    }
}

void FrSaveGraphTabSettingsCmd::SaveGraphs(QDomElement &parent, std::vector<FrGraphDocObj*> graphs){
    for(int i = 0; i < graphs.size(); ++i){
        FrGraphDocObj* graphDO = (FrGraphDocObj*) graphs[i];

        QDomElement graphElem = m_Document->createElement(FR_XML_GRAPH_ELEM);
        graphElem.setAttribute(FR_XML_TIMESERIA_ATTR, graphDO->GetTimeSeria());
    
        // TODO: save graph settings
        QDomElement gsElem = m_Document->createElement(FR_XML_GRAPH_SETTINGS_ELEM);
        FrGraphSettings* gs = graphDO->GetSettings();
        gsElem.setAttribute(FR_XML_NAME_ATTR, gs->Name);        

        QColor color = gs->Color;
        QString colorValue = "#";
        colorValue += GET_HEX_COLOR_STRING(color.red());
        colorValue += GET_HEX_COLOR_STRING(color.green());
        colorValue += GET_HEX_COLOR_STRING(color.blue());
        
        gsElem.setAttribute(FR_XML_COLOR_ATTR, colorValue);        
        gsElem.setAttribute(FR_XML_VISIBLE_ATTR, (gs->Visibility ? 1 : 0));        
        gsElem.setAttribute(FR_XML_DRAWMETHOD_ATTR, (int)gs->DrawMethod);        

        FrGraphSettings::GraphTypes gtype = graphDO->GetSettings()->GetType();
        gsElem.setAttribute(FR_XML_GRAPHTYPE_ATTR, (int)gtype);

        int I, J, K;
        QDomElement posElem;

        switch(gtype){
            case FrGraphSettings::GT_Intencity:
                I = ((FrIntencityGraphSettings*)gs)->I;
                J = ((FrIntencityGraphSettings*)gs)->J;
                K = ((FrIntencityGraphSettings*)gs)->K;

                posElem = m_Document->createElement(FR_XML_GRAPH_VOXEL_ELEM);
                posElem.setAttribute(FR_XML_X_ATTR, I);
                posElem.setAttribute(FR_XML_Y_ATTR, J);
                posElem.setAttribute(FR_XML_Z_ATTR, K);
                gsElem.appendChild(posElem);
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

        graphElem.appendChild(gsElem);
        parent.appendChild(graphElem);
    }
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrSaveGraphTabSettingsCmd::CanUndo(){
    return false;
}

bool FrSaveGraphTabSettingsCmd::Undo(){
    return false;
}

bool FrSaveGraphTabSettingsCmd::Redo(){
    return false;
}
