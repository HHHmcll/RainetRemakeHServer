#include "InitializeTerminal.h"
#include "Enums.h"

void CA_InitializeTerminal::Initialize(EPlayerType owner, void* meta)
{
}

bool CA_InitializeTerminal::CanDo(RSData_Command& command, RSData_Map& map)
{
	return false;
}

bool CA_InitializeTerminal::Do(RSData_Command& command, RSData_Map& map, std::vector<uint8_t>& outputBuffer)
{
	return false;
}

bool CA_InitializeTerminal::Usable()
{
	return false;
}

RS_CommandAction* CreateInitializeTerminal() {
	static CA_InitializeTerminal instance = CA_InitializeTerminal();
	return &instance;
}

RS_CommandActionCreateFunction createInitializeTerminalFunction = RS_CommandActionCreateFunction(EActionType::LineBoost, &CreateInitializeTerminal);