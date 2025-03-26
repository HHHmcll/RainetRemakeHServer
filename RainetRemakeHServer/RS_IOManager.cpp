#include "RS_IOManager.h"
#include "RSData_Command.h"
#include <vector>
#include <iostream>


RS_IOManager& RS_IOManager::GetInstance() {
    static RS_IOManager instance = RS_IOManager();
    return instance;
}

void RS_IOManager::Init() {

    #ifdef _DEBUG
    freopen("input.bin", "rb", stdin);
    freopen("output.bin", "wb", stdout);
    #endif
    GetInstance();
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

bool RS_IOManager::ShouldResolve()
{
    return !GetInstance().OutputQueue.empty() && !GetInstance().HoldFlag;
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

    case EActionType::VisualEffet:
    case EActionType::InitializeBoard:
    {
        for (uint8_t& byte : command.Data.RawByte) {
            byte = std::cin.get();
        }

        uint32_t numTerminal = command.Data.TerminalSetup;
        auto* terminalData = new std::vector<uint8_t>();
        terminalData->reserve(numTerminal);
        command.Meta = std::shared_ptr<void>(terminalData);
        for (uint32_t i = 0; i < numTerminal; i++) {
            terminalData->push_back(std::cin.get());
        }
        break;
    }
    case EActionType::Move:
    case EActionType::LineBoost:
    case EActionType::FireWall:
    case EActionType::VirusCheck:
    case EActionType::NotFound:
    case EActionType::RabbitTrap:
    case EActionType::ZeroDayAttack:
    case EActionType::SandBox:
    {
        for (uint8_t& byte : command.Data.RawByte) {
            byte = std::cin.get();
        }
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
        if(GetInstance().OutputQueue.empty()){
            return;
        }
        auto& data = GetInstance().OutputQueue.front();
        GetInstance().WriteData(data.data, data.dataSize);
        GetInstance().OutputQueue.pop();
    }
}
