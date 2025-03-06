#include "LineBoost.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

CA_LineBoost::CA_LineBoost() {}

std::shared_ptr<RS_CommandAction> CA_LineBoost::CreateNewObject(void* meta)
{
	return std::shared_ptr<RS_CommandAction>(new CA_LineBoost());
}


bool CA_LineBoost::CanDo(RSData_Command& command, RSData_Map& map)
{
    // check whether wil be blocked by other cards
    if (map.IsBlockedByAnyTerminal(command)) {
        return false;
    }
    if (installedPiece) {
        RSData_Piece* piece = map.getPiece(command.Data.Coordinate.row1, command.Data.Coordinate.col1);
        if (piece != installedPiece) {
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
    if (installedPiece) {
        // remove the installed lb
        installedPiece = nullptr;
    }
    else {
        // install the lb on the selected piece
        installedPiece = map.getPiece(command.Data.Coordinate.row1, command.Data.Coordinate.col1);
    }
    return true;
}

bool CA_LineBoost::Block(RSData_Command& command, RSData_Map& map)
{

    uint8_t& row1 = command.Data.Coordinate.row1;
    uint8_t& col1 = command.Data.Coordinate.col1;

    // SandBox only can be used on the piece installed lineboost
    if (command.ActionType == EActionType::SandBox)
    {
        // block SB if the selected piece is not installed on lineboost
        if(!installedPiece || installedPiece != map.getPiece(row1, col1) || installedPiece->Player->PlayerID != command.Player)
        {
            return true; 
        }
    }
    return false;
}


RS_CommandAction* GetStaticLineBoost() {
    static CA_LineBoost LineBoostStatic = CA_LineBoost();
    return &LineBoostStatic;
}


RS_CommandActionCreateFunction createLineBoostFunction = RS_CommandActionCreateFunction(EActionType::LineBoost, &GetStaticLineBoost);