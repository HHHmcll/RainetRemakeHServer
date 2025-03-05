#pragma once
#include "Enums.h"
#include <memory>
struct RSData_Command {

	EActionType terminalType;
	EPlayerType player;
	union 
	{
		struct
		{
			uint8_t col1, row1;
			uint8_t col2, row2;
		} Coordinate;
		struct 
		{
			uint8_t num;
			uint8_t buffer1;
			uint16_t buffer2;
		}TerminalSetup;
		struct 
		{
			uint8_t setup;
			uint8_t meta1;
			uint8_t meta2;
			uint8_t meta3;
		}PieceSetup;
	} data;
	

	std::shared_ptr<void> meta;
};