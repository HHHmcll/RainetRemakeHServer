#pragma once

#include "RS_CommandActionManager.h"
#include <random>

class CA_BoardDisplay :public RS_CommandAction {
private:
	std::default_random_engine engine;
	CA_BoardDisplay();
	friend CA_BoardDisplay* GetBoardDisplay();
public:
	bool CanDo(const RSData_Command& command, const RSData_Map& map) const override;
	bool Do(RSData_Command& command, RSData_Map& map)const override;

};
