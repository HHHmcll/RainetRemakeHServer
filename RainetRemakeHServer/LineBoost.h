#pragma once
#include "RS_CommandActionManager.h"

class CA_LineBoost :public RS_CommandAction {
private:
	CA_LineBoost();
	friend RS_CommandAction* CreateLineBoost();
public:
	void Initialize(EPlayerType owner, void* meta) override;
	bool CanDo(RSData_Command& command, RSData_Map& map) override;
	bool Do(RSData_Command& command, RSData_Map& map, std::vector<uint8_t>& outputBuffer) override;
	bool Usable() override;
};
