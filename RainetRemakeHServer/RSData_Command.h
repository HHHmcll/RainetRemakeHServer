#pragma once
#include "Enums.h"
#include <memory>
struct RSData_Command {

	EActionType ActionType;
	EPlayerType Player;
	union 
	{
		struct
		{
			uint8_t col1, row1;
			uint8_t col2, row2;
		} Coordinate;
		uint32_t TerminalSetup;
		struct 
		{
			uint8_t setup;
			uint8_t meta1;
			uint8_t meta2;
			uint8_t meta3;
		}PieceSetup;
		struct
		{
			uint8_t byte1;
			uint8_t byte2;
			uint8_t byte3;
			uint8_t byte4;
		}Raw;
		
	} Data;
	
	std::shared_ptr<void> Meta;
	RSData_Command():ActionType(EActionType::Error),Player(EPlayerType::Empty),Data{0},Meta(nullptr){

	}
};