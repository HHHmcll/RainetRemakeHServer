#pragma once
#include <fstream>
struct RSData_Command;
class RS_Logger {
	std::ofstream logFile;
public:
	RS_Logger(const char* path);
	void LogMove(RSData_Command& move);
	void Save()
};