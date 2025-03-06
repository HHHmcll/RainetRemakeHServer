#include "RS_Logger.h"
#include "RSData_Command.h"

RS_Logger::RS_Logger(const char* path):
	logFile(path)
{}

void RS_Logger::LogMove(RSData_Command& move)
{
	logFile.write((const char*)&move, sizeof(move));
}
void RS_Logger::Save()
{

}
