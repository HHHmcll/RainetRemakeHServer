#include "LineBoost.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

CA_LineBoost::CA_LineBoost():
    InstalledPiece(nullptr){}

std::shared_ptr<RS_CommandAction> CA_LineBoost::CreateNewObject(void* meta)
{
	return std::shared_ptr<RS_CommandAction>(new CA_LineBoost());
}


bool CA_LineBoost::CanDo(RSData_Command& command, RSData_Map& map)
{
    // check whether wil be blocked by other cards
    if (map.ForEachPlayer([&](RSData_Player* player)->bool{
        return player->ForEachTerminal([&](RS_CommandAction* action)->bool {
            return action->Block(player, command, map) != EBlock_Status::Block;
        });
    })) {
        return false;
    }
    if (InstalledPiece) {
        RSData_Piece* piece = map.getPiece(command.Data.Coordinate.row1, command.Data.Coordinate.col1);
        if (piece != InstalledPiece) {
            return false;
        }
    }
    else {
        if (!map.getPiece(command.Data.Coordinate.row1, command.Data.Coordinate.col1)) {
            return false;
        }
    }
    return true;
}
bool CA_LineBoost::Do(RSData_Command& command, RSData_Map& map)
{
    // check whether the piece is installed
    if (InstalledPiece) {
        // remove the installed lb
        InstalledPiece = nullptr;
    }
    else {
        // install the lb on the selected piece
        InstalledPiece = map.getPiece(command.Data.Coordinate.row1, command.Data.Coordinate.col1);
    }
    return true;
}

EBlock_Status CA_LineBoost::Block(RSData_Player* owner, RSData_Command& command, RSData_Map& map)
{
    
    uint8_t& row1 = command.Data.Coordinate.row1;
    uint8_t& col1 = command.Data.Coordinate.col1;

    switch (command.ActionType)
    {
    case EActionType::SandBox:
        // SandBox only can be used on the piece installed lineboost
        if (owner && command.Player == owner->PlayerID)
        {
            if (!map.ForEachPlayer([&](RSData_Player* player)
                {
                    if (!player->Cards[EActionType::LineBoost])
                    {
                        return false;
                    }
                    // Can SB if the selected piece is installed on lineboost
                    CA_LineBoost* card = dynamic_cast<CA_LineBoost*>(player->Cards[EActionType::LineBoost].get());
                    if (card->InstalledPiece && card->InstalledPiece == map.getPiece(row1, col1))
                    {
                        return true;
                    }
                    return false;
                }
            )) 
            {
                return EBlock_Status::Block;
            }

        }
    default:
        break;
    }
    return EBlock_Status::Ignored;
}



RS_CommandAction* GetStaticLineBoost() {
    static CA_LineBoost LineBoostStatic = CA_LineBoost();
    return &LineBoostStatic;
}


RS_CommandActionCreateFunction createLineBoostFunction = RS_CommandActionCreateFunction(EActionType::LineBoost, &GetStaticLineBoost);