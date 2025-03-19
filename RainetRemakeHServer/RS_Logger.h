#pragma once
#include "Enums.h"
#include <fstream>
#include <vector>

struct RSData_Command;
class RS_Logger {
	std::ofstream logFile;
	std::vector<RSData_Command> RecordedMoves;
public:
	RS_Logger(const char* path);
	void LogMove(RSData_Command& move);
	void Save();
	void LogEnd();
};