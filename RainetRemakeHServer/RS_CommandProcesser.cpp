#include "RS_CommandProcesser.h"
#include "RS_CommandActionManager.h"
#include "RSData_Command.h"
#include "RS_IOManager.h"
#include "RSData_Map.h"

bool RS_CommandProcesser::ProcessCommand(RSData_Command &command, RSData_Map &map)
{
    const RS_CommandAction *action = RS_CommandActionManager::GetStaticAction(command.ActionType);

    if(map.GetGameState() == EGameState::EndGame){
        if(command.ActionType != EActionType::GetOutput && command.ActionType != EActionType::VisualEffet){
            return false;
        }
    }
    if (action && action->CanDo(command, map))
    {
        return action->Do(command, map);
    } else {
        struct Error{
            EPlayerType retPlayer;
            EActionType type = EActionType::Error;
        }* out = new Error();
        out->retPlayer = command.Player;
        RS_IOManager::QueueOutput(reinterpret_cast<uint8_t*>(out), sizeof(Error));
        return false;
    }
}