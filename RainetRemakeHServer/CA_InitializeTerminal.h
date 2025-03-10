#pragma once
#include "RS_CommandActionManager.h"

class CA_InitializeTerminal :public RS_CommandAction {
private:
	bool Initialized[2] = {0};
	CA_InitializeTerminal();
	friend CA_InitializeTerminal* GetStaticInitializeTerminal();
public:
	static const EActionType StaticType = EActionType::InitializeTerminal;
	bool CanDo(RSData_Command& command, RSData_Map& map) const override;
	bool Do(RSData_Command& command, RSData_Map& map) const override;
	bool Initalized() const;
};
