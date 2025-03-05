#include "RS_IOManager.h"
#include "RSData_Command.h"
#include <vector>
#include <iostream>

void RS_IOManager::FetchCommand(RSData_Command& command)
{
    
    command.Player = EPlayerType (std::cin.get());
    command.ActionType = EActionType(std::cin.get());

    switch (command.ActionType) {
    case EActionType::BoardDisplay:
    case EActionType::Error:
    
        break;
    case EActionType::InitializePieces:
        command.Data.PieceSetup.setup = std::cin.get();

        break;
    case EActionType::VisualEffet:
    case EActionType::InitializeTerminal:
        command.Data.Raw.byte1 = std::cin.get();
        command.Data.Raw.byte2 = std::cin.get();
        command.Data.Raw.byte3 = std::cin.get();
        command.Data.Raw.byte4 = std::cin.get();

        uint32_t numTerminal = command.Data.TerminalSetup;
        auto * terminalData = new std::vector<EActionType>();
        terminalData->reserve(numTerminal);
        command.Meta = std::shared_ptr<void>((void*)(terminalData));
        for(int i= 0;i<numTerminal;i++){
            terminalData->push_back(EActionType(std::cin.get()));
        }
        break;
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
