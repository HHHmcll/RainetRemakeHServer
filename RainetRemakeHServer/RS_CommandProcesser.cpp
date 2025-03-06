#include "RS_CommandProcesser.h"
#include "RS_CommandActionManager.h"
#include "RSData_Command.h"

bool RS_CommandProcesser::ProcessCommand(RSData_Command& command, RSData_Map& map)
{
    RS_CommandAction* action = RS_CommandActionManager::GetStaticAction(command.ActionType);
    if(action->CanDo(command,map)){
        return action->Do(command,map);
    }
    return false;
}