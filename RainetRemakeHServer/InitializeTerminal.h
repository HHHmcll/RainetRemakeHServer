#pragma once
#include "RS_CommandActionManager.h"

class CA_InitializeTerminal :public RS_CommandAction {
private:
	bool Initialized[2] = {0};
	CA_InitializeTerminal();
	friend RS_CommandAction* CreateInitializeTerminal();
public:
	std::shared_ptr<RS_CommandAction> CreateNewObject(void* meta) override;
	bool CanDo(RSData_Command& command, RSData_Map& map) override;
	bool Do(RSData_Command& command, RSData_Map& map, std::vector<uint8_t>& outputBuffer) override;
	bool Block(RSData_Command& command, RSData_Map& map) override;
};
