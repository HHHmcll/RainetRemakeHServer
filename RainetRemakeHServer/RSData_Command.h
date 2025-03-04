#pragma once
#include "Enums.h"
struct RSData_Command {

	EActionType terminalType;
	EPlayerType player;
	uint8_t col1, row1;
	uint8_t col2, row2;

	void* meta;
};