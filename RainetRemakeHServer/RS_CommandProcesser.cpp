#include "RS_CommandProcesser.h"
#include "RS_CommandActionManager.h"
#include "RSData_Command.h"

bool RS_CommandProcesser::ProcessCommand(RSData_Command& command, RSData_Map& map, std::vector<uint8_t>& output)
{
    RS_CommandAction* action = RS_CommandActionManager::GetStaticAction(command.ActionType);
    if(action->CanDo(command,map)){
        return action->Do(command,map,output);
    }
}