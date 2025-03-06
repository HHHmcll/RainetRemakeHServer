#include "RS_IOManager.h"
#include "RSData_Command.h"
#include <vector>
#include <iostream>

RS_IOManager& RS_IOManager::GetInstance() {
    static RS_IOManager instance = RS_IOManager();
    return instance;
}

void RS_IOManager::Init() {

}

void RS_IOManager::WriteData(uint8_t* data, size_t dataSize)
{
    std::cout.write((const char*)data, dataSize);
    delete[] data;
    GetInstance().HoldFlag = true;
}

bool RS_IOManager::HasCommand()
{
    return !std::cin.eof();
}

void RS_IOManager::FetchCommand(RSData_Command& command)
{
    
    command.Player = EPlayerType (std::cin.get());
    command.ActionType = EActionType(std::cin.get());

    switch (command.ActionType) {
    case EActionType::GetOutput:
    case EActionType::BoardDisplay:
    case EActionType::Error:
    
        break;
    case EActionType::InitializePieces:
        command.Data.PieceSetup.setup = std::cin.get();

        break;
    case EActionType::VisualEffet:
    case EActionType::InitializeTerminal:
    {
        command.Data.Raw.byte1 = std::cin.get();
        command.Data.Raw.byte2 = std::cin.get();
        command.Data.Raw.byte3 = std::cin.get();
        command.Data.Raw.byte4 = std::cin.get();

        uint32_t numTerminal = command.Data.TerminalSetup;
        auto* terminalData = new std::vector<EActionType>();
        terminalData->reserve(numTerminal);
        command.Meta = std::shared_ptr<void>((void*)(terminalData));
        for (uint32_t i = 0; i < numTerminal; i++) {
            terminalData->push_back(EActionType(std::cin.get()));
        }
        break;
    }
    case EActionType::Move:
    case EActionType::LineBoost:
    case EActionType::FireWall:
    case EActionType::VirusCheck:
    case EActionType::NotFound:
    case EActionType::Rabbit:
    case EActionType::ZeroDayAttack:
    case EActionType::SandBox:
    {
        command.Data.Coordinate.col1 = std::cin.get();
        command.Data.Coordinate.row1 = std::cin.get();

        command.Data.Coordinate.col2 = std::cin.get();
        command.Data.Coordinate.row2 = std::cin.get();
        break;
    }
    default:

        break;
    }

}



void RS_IOManager::QueueOutput(uint8_t* data, size_t dataSize)
{
    if (GetInstance().HoldFlag){
        GetInstance().OutputQueue.push({ data, dataSize });
    }
    else {
        GetInstance().WriteData(data, dataSize);
    }
}

void RS_IOManager::Unhold()
{
    GetInstance().HoldFlag = false;
}

void RS_IOManager::ResolveOutput()
{
    if (!GetInstance().HoldFlag) {
        auto& data = GetInstance().OutputQueue.front();
        GetInstance().WriteData(data.data, data.dataSize);
        GetInstance().OutputQueue.pop();
    }
}
