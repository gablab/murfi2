#include "FrMultiCmd.h"


FrMultiCmd::FrMultiCmd(){
}

FrMultiCmd::~FrMultiCmd(){
    // Clear internal list
    CmdList::iterator it, itEnd(m_commands.end());
    for(it = m_commands.begin(); it != itEnd; ++it){
        delete (*it);
    }
    m_commands.clear();
}

bool FrMultiCmd::Execute(){
    bool result = true;

    CmdList::iterator it, itEnd(m_commands.end());
    for(it = m_commands.begin(); it != itEnd; ++it){
        result &= (*it)->Execute();
    }
    return result;
}

bool FrMultiCmd::CanUndo(){
    bool result = true;

    CmdList::iterator it, itEnd(m_commands.end());
    for(it = m_commands.begin(); it != itEnd; ++it){
        result &= (*it)->CanUndo();
    }
    return result;
}

bool FrMultiCmd::Undo(){
    bool result = true;

    CmdList::iterator it, itEnd(m_commands.end());
    for(it = m_commands.begin(); it != itEnd; ++it){
        result &= (*it)->Undo();
    }
    return result;
}

bool FrMultiCmd::Redo(){
    bool result = true;

    CmdList::iterator it, itEnd(m_commands.end());
    for(it = m_commands.begin(); it != itEnd; ++it){
        result &= (*it)->Redo();
    }
    return result;
}

void FrMultiCmd::AddCommand(FrBaseCmd* cmd){
    m_commands.push_back(cmd);
}
