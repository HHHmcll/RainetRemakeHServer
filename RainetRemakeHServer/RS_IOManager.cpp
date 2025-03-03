#include "RS_IOManager.h"
#include "RSData_Command.h"
#include <iostream>

void RS_IOManager::FetchCommand(RSData_Command& command)
{
    unsigned char* data = (unsigned char*)&command;

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
    case EActionType::InitializeTerminal:
        command.col1 = std::cin.get();

        break;
    case EActionType::Move:
    case EActionType::LineBoost:
    case EActionType::FireWall:
    case EActionType::VirusCheck:
    case EActionType::NotFound:
    case EActionType::Rabbit:
    case EActionType::ZeroDayAttack:
    case EActionType::SandBox:

        command.col1 = std::cin.get();
        command.row1 = std::cin.get();

        command.col2 = std::cin.get();
        command.row2 = std::cin.get();
        break;
    default:

        break;
    }

}
