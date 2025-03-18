#pragma once
#include "RS_CommandActionManager.h"

struct RSData_Piece;

class CA_FireWall :public RS_TerminalCard {
private:
	CA_FireWall();
	friend const RS_CommandAction* GetStaticFireWall();
	
public:

	RSData_Slot* TrappedSlot;
	static const EActionType StaticType = EActionType::FireWall;
	std::shared_ptr<RS_TerminalCard> CreateNewObject(void* meta) const override;
	bool CanDo(const RSData_Command& command, const RSData_Map& map) const override;
	bool Do(RSData_Command& command, RSData_Map& map) const override;
	bool Is(const RSData_Slot* slot) const override;
};

