#pragma once

#include "RS_CommandActionManager.h"

class CA_VisualEffet :public RS_CommandAction {
private:

	CA_VisualEffet() = default;
	friend const RS_CommandAction* GetStaticVisualEffet();
public:
	bool CanDo(const RSData_Command& command, const RSData_Map& map) const override;
	bool Do(RSData_Command& command, RSData_Map& map)const override;

};
