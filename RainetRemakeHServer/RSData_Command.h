#pragma once
#include "Enums.h"
#include <memory>
struct RSData_Command {

	EPlayerType Player;
	EActionType ActionType;
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

		uint8_t RawByte[4];

	} Data;
	
	std::shared_ptr<void> Meta;
	RSData_Command():ActionType(EActionType::Error),Player(EPlayerType::Empty),Data{0},Meta(nullptr){

	}
};