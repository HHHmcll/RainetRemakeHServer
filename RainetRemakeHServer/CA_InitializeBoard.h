#pragma once
#include "RS_CommandActionManager.h"

class CA_InitializeBoard :public RS_CommandAction {
private:
	bool Initialized[2] = {0};
	CA_InitializeBoard();
	friend CA_InitializeBoard* GetStaticInitializeBoard();
public:
	static const EActionType StaticType = EActionType::InitializeBoard;
	bool CanDo(const RSData_Command& command, const RSData_Map& map) const override;
	bool Do(RSData_Command& command, RSData_Map& map) const override;
	bool Initalized() const;
};
