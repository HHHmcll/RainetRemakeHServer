#include "RS_IOManager.h"
#include "RSData_Command.h"
#include <iostream>

void RS_IOManager::FetchCommand(RSData_Command& command)
{
    uint8_t* data = (uint8_t*)&command;

    data[0] = std::cin.get();
    data[1] = std::cin.get();

    switch (command.terminalType) {
    case EActionType::BoardDisplay:
    case EActionType::AddPlayer:
    case EActionType::ErrorRequirements:
        break;
    case EActionType::ArtRequirements:

        break;
    case EActionType::InitializePieces:
        command.data.PieceSetup.data = std::cin.get();

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

        command.data.Coordinate.col1 = std::cin.get();
        command.data.Coordinate.row1 = std::cin.get();

        command.data.Coordinate.col2 = std::cin.get();
        command.data.Coordinate.row2 = std::cin.get();
        break;
    default:

        break;
    }

}
