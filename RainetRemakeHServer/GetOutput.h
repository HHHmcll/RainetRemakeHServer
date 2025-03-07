#pragma once

#include "RS_CommandActionManager.h"

class CA_GetOutput :public RS_CommandAction {
private:

	CA_GetOutput() = default;
	friend RS_CommandAction* GetStaticGetOutput();
public:
	std::shared_ptr<RS_CommandAction> CreateNewObject(void* meta) override;
	bool CanDo(RSData_Command& command, RSData_Map& map) override;
	bool Do(RSData_Command& command, RSData_Map& map) override;
	bool Block(RSData_Player* owner, RSData_Command& command, RSData_Map& map) override;
};
