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
        case EActionType::VisualEffet:
            return "VisualEffet";
        case EActionType::InitializeBoard:
            return "InitializeBoard";
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

        case EActionType::InitializeBoard: 
        {
            logFile << ActionTypeToString(Data.ActionType);
            std::vector<uint8_t>& SetupData = *(reinterpret_cast<std::vector<uint8_t>*>(Data.Meta.get()));

            int setupPTR = 0;
            int CardNum = (Data.Data.TerminalSetup - 16) / 2;
            for (int p = 0; p < 2;p++) {
                logFile << PlayerTypeToString(EPlayerType(p));

                for (int i = 0; i < 8; i++, setupPTR++) {
                    logFile << "\t" << PieceTypeToString(SetupData[setupPTR] == 'L' ? EPieceType::Link : EPieceType::Virus);
                }

                for (int i = 0; i < CardNum; i++, setupPTR++) {
                    EActionType termType = EActionType(SetupData[setupPTR]);
                    logFile << "\t" << ActionTypeToString(termType);
                }
                logFile << "\n";
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

void RS_Logger::LogEnd(){
    logFile << "GameEnd" << std::endl;
}