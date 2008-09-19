#include "FrMultiCmd.h"

#define FOR_EACH_CMD(Action,Result)                     \
    CmdList::iterator it, itEnd(m_commands.end());      \
    for(it = m_commands.begin(); it != itEnd; ++it){    \
        Result &= (*it)->##Action();                    \
    }

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

    FOR_EACH_CMD(Execute, result);
    return result;
}

bool FrMultiCmd::CanUndo(){
    bool result = true;

    FOR_EACH_CMD(CanUndo, result);
    return result;
}

bool FrMultiCmd::Undo(){
    bool result = true;

    FOR_EACH_CMD(Undo, result);
    return result;
}

bool FrMultiCmd::Redo(){
    bool result = true;

    FOR_EACH_CMD(Redo, result);
    return result;
}

void FrMultiCmd::AddCommand(FrBaseCmd* cmd){
    m_commands.push_back(cmd);
}