#include "RS_IOManager.h"
#include "RSData_Command.h"
#include <iostream>

void RS_IOManager::FetchCommand(RSData_Command& command)
{
    uint8_t* Data = (uint8_t*)&command;

    Data[0] = std::cin.get();
    Data[1] = std::cin.get();

    switch (command.ActionType) {
    case EActionType::BoardDisplay:
    case EActionType::AddPlayer:
    case EActionType::Error:
        break;
    case EActionType::VisualEffet:

        break;
    case EActionType::InitializePieces:
        command.Data.PieceSetup.setup = std::cin.get();

        break;
    case EActionType::InitializeTerminal:
        break;;
    case EActionType::Move:
    case EActionType::LineBoost:
    case EActionType::FireWall:
    case EActionType::VirusCheck:
    case EActionType::NotFound:
    case EActionType::Rabbit:
    case EActionType::ZeroDayAttack:
    case EActionType::SandBox:

        command.Data.Coordinate.col1 = std::cin.get();
        command.Data.Coordinate.row1 = std::cin.get();

        command.Data.Coordinate.col2 = std::cin.get();
        command.Data.Coordinate.row2 = std::cin.get();
        break;
    default:

        break;
    }

}
