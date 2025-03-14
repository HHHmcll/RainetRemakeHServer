#include "RS_Logger.h"
#include "RSData_Command.h"
#include "RSData_Map.h"

RS_Logger::RS_Logger(const char* path):
	logFile(path)
{}

void RS_Logger::LogMove(RSData_Command& move)
{
	RecordedMoves.push_back(move);
}

constexpr const char* ActionTypeToString(const EActionType type){

    switch (type)
    {
        case EActionType::GetOutput:
            return "GetOutput";
        case EActionType::BoardDisplay:
            return "BoardDisplay";
        case EActionType::Error:
            return "Error";
        case EActionType::InitializePieces:
            return "InitializePieces";
        case EActionType::VisualEffet:
            return "VisualEffet";
        case EActionType::InitializeTerminal:
            return "InitializeTerminal";
        case EActionType::Move:
            return "Move";
        case EActionType::LineBoost:
            return "LineBoost";
        case EActionType::FireWall:
            return "FireWall";
        case EActionType::VirusCheck:
            return "VirusCheck";
        case EActionType::NotFound:
            return "NotFound";
        case EActionType::RabbitTrap:
            return "Rabbit";
        case EActionType::ZeroDayAttack:
            return "ZeroDayAttack";
        case EActionType::SandBox:
            return "SandBox";
        default:
            return "UNDEFINED";
    }
}

constexpr const char* PlayerTypeToString(const EPlayerType type) {
    switch (type)
    {

    case Player1:
        return "Player1";
    case Player2:
        return "Player2";
    case Viewer:
    case God:
    case Everyone:
    case Empty:
    default:
        return "ERRORPLAYER";
        break;
    }
}

constexpr const char* PieceTypeToString(const EPieceType type) {
    switch (type)
    {
    case Link:
        return "Link";
    case Virus:
        return "Virus";
    default:
        return "UNDEFINED";
        break;
    }
}

void RS_Logger::Save()
{
	for (RSData_Command& Data : RecordedMoves) {
		switch (Data.ActionType)
		{
        case EActionType::GetOutput:
        case EActionType::BoardDisplay:
        case EActionType::Error:
        case EActionType::VisualEffet:
            break;

        case EActionType::InitializePieces:

            logFile << PlayerTypeToString(Data.Player) << "\t" << ActionTypeToString(Data.ActionType);
            for (int i = 0; i < MAP_SIZE; i++)
            {
                logFile << "\t" << PieceTypeToString(EPieceType(Data.Data.PieceSetup.setup & 1 << i));
            }
            logFile << "\n";
            break;
        case EActionType::InitializeTerminal: 
        {
            logFile << PlayerTypeToString(Data.Player) << "\t" << ActionTypeToString(Data.ActionType);
            std::vector<EActionType>* terminals = static_cast<std::vector<EActionType>*>(Data.Meta.get());
            for (EActionType terminal : *terminals) {
                logFile << "\t" << ActionTypeToString(terminal);
            }
            logFile << "\n";
        }
            break;
        case EActionType::Move:
        case EActionType::LineBoost:
        case EActionType::FireWall:
        case EActionType::VirusCheck:
        case EActionType::NotFound:
        case EActionType::RabbitTrap:
        case EActionType::ZeroDayAttack:
        case EActionType::SandBox:
            logFile << PlayerTypeToString(Data.Player) << "\t" << ActionTypeToString(Data.ActionType) << "\t";
            logFile << Data.Data.RawByte[0] << "\t" << Data.Data.RawByte[1] << "\t" << Data.Data.RawByte[2] << "\t" << Data.Data.RawByte[3] << "\n";
            break;
		default:
			break;
		}
	}
}
