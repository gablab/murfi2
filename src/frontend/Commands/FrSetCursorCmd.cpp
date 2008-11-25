#include "FrSetCursorCmd.h"
#include "FrToolController.h"
#include "FrMainWindow.h"
#include "QVTKWidget.h"

// Qt stuff
#include "Qt/qcursor.h"


FrSetCursorCmd::FrSetCursorCmd()
: m_CursorType(Unknown){
}

bool FrSetCursorCmd::Execute(){
    bool result = false;

    // Check params
    if(!this->GetMainController() || m_CursorType == Unknown)
        return false;

    FrMainController* mc = this->GetMainController();
    FrMainWindow* mw = mc->GetMainView();
    QVTKWidget* qvtkWidget = mw->GetQVTKWidget();
    
    QCursor cursor(Qt::ArrowCursor);

    // set appropriate cursor
    switch(m_CursorType){
        case FrSetCursorCmd::Zoom:
            cursor.setShape(Qt::SizeVerCursor);
            break;
        case FrSetCursorCmd::Pan:
            cursor.setShape(Qt::OpenHandCursor);
            break;
        default:
            // do nothing here
            break;
    }

    qvtkWidget->setCursor(cursor);

    return result;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrSetCursorCmd::CanUndo(){
    return false;
}

bool FrSetCursorCmd::Undo(){
    return false;
}

bool FrSetCursorCmd::Redo(){
    return false;
}
