#pragma once
struct RSData_Command;
class RS_Logger {
public:
	RS_Logger(const char* path);
	void LogMove(RSData_Command& move);
};