#pragma once
#include "Enums.h"

#include <queue>

struct RSData_Command;

class RS_IOManager {
	RS_IOManager() = default;
	static RS_IOManager& GetInstance();
	struct OutputData
	{
		uint8_t* data;
		size_t dataSize;
	};
	std::queue<OutputData> OutputQueue;
	bool HoldFlag = false;
	void WriteData(uint8_t* data, size_t dataSize);
public:
	static bool HasCommand();
	static void FetchCommand(RSData_Command& command);
	static void QueueOutput(uint8_t* data, size_t dataSize);
	static void Unhold();
	static void ResolveOutput();
	static void Init();
};