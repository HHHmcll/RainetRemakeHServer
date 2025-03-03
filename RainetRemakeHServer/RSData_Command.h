#pragma once
#include "Enums.h"
struct RSData_Command {

	EActionType terminalType;
	EPlayerType player;
	unsigned char col1, row1;
	unsigned char col2, row2;

	void* meta;
};