#include "Move.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

CA_Move::CA_Move():
    InstalledPiece(nullptr){}

std::shared_ptr<RS_CommandAction> CA_Move::CreateNewObject(void* meta)
{
	return std::shared_ptr<RS_CommandAction>(new CA_Move());
}


bool CA_Move::CanDo(RSData_Command& command, RSData_Map& map)
{
    // check whether wil be blocked by other cards
    if (map.ForEachPlayer([&](RSData_Player* player)->bool{
        return player->ForEachTerminal([&](RS_CommandAction* action)->bool {
            EBlock_Status BlockStatus = action->Block(player, command, map);
            if (BlockStatus == EBlock_Status::Ignored) {
                return false;
            }

            return false;
        });
    })) {
        return false;
    }

    bool PassedOnce = false;
    // check whether wil be blocked by other cards
    if (map.ForEachPlayer([&](RSData_Player* player)->bool {
        return player->ForEachTerminal([&](RS_CommandAction* action)->bool {
            EBlock_Status BlockStatus = action->Block(player, command, map);
            if (BlockStatus == EBlock_Status::Ignored) {
                return false;
            }
            else if (BlockStatus == EBlock_Status::Pass) {
                PassedOnce = true;
                return false;
            }

            return false;
            });
        })) {
        return false;
    }

    // if only 1 step is moved, check normal move after checking all terminals
    if (abs(command.Data.Coordinate.row1 - command.Data.Coordinate.row2) + abs(command.Data.Coordinate.col1 - command.Data.Coordinate.col2) == 1) {

        if (!PassedOnce) {
            RSData_Piece** SlotFrom = map.getPieceSlot(command.Data.Coordinate.row1, command.Data.Coordinate.col1);
            if (!SlotFrom) {
                return false;
            }


            if (command.Data.Coordinate.row2 == -1 || command.Data.Coordinate.row2 == MAP_SIZE) {
                if (command.Data.Coordinate.col2 == 3 || command.Data.Coordinate.col2 == 4) {

                }
            }
            RSData_Piece** SlotTo = map.getPieceSlot(command.Data.Coordinate.row2, command.Data.Coordinate.col2);
            if () {

            }
            map.getPieceSlot(command.Data.Coordinate.row2, command.Data.Coordinate.col2);
        }

    }
    // Moving multiple steps will only be accepted if a terminal card accepts it
    else {
        if (!PassedOnce) {
            return false;
        }
    }


    return true;
}
bool CA_Move::Do(RSData_Command& command, RSData_Map& map)
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

EBlock_Status CA_Move::Block(RSData_Player* owner, RSData_Command& command, RSData_Map& map)
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
                    if (!player->Cards[EActionType::Move])
                    {
                        return false;
                    }
                    // Can SB if the selected piece is installed on lineboost
                    CA_Move* card = dynamic_cast<CA_Move*>(player->Cards[EActionType::Move].get());
                    if (card->InstalledPiece && card->InstalledPiece == map.getPiece(row1, col1))
                    {
                        return true;
                    }
                    return false;
                }
            )) 
            {
                return true;
            }

        }
    default:
        break;
    }
    return false;
}



RS_CommandAction* GetStaticMove() {
    static CA_Move MoveStatic = CA_Move();
    return &MoveStatic;
}


RS_CommandActionCreateFunction createMoveFunction = RS_CommandActionCreateFunction(EActionType::Move, &GetStaticMove);