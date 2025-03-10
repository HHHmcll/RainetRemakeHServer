#pragma once

#include "RS_CommandActionManager.h"

class CA_GetOutput :public RS_CommandAction {
private:

	CA_GetOutput() = default;
	friend const RS_CommandAction* GetStaticGetOutput();
public:
	bool CanDo(RSData_Command& command, RSData_Map& map) const override;
	bool Do(RSData_Command& command, RSData_Map& map)const override;

};
