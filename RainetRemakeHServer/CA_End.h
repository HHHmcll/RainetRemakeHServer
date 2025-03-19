#pragma once

#include "RS_CommandActionManager.h"

class CA_End :public RS_CommandAction {
private:

	CA_End() = default;
	friend const RS_CommandAction* GetStaticEnd();
public:
	bool CanDo(const RSData_Command& command, const RSData_Map& map) const override;
	bool Do(RSData_Command& command, RSData_Map& map)const override;

};
