#pragma once
#include "RS_CommandActionManager.h"

struct RSData_Piece;

class CA_SandBox :public RS_TerminalCard {
private:
	CA_SandBox();
	friend const RS_CommandAction* GetStaticSandBox();
	bool used;


public:
	RSData_Slot* TrappedSlot;
	static const EActionType StaticType = EActionType::SandBox;
	std::shared_ptr<RS_TerminalCard> CreateNewObject(void* meta) const override;
	bool CanDo(const RSData_Command& command, const RSData_Map& map) const override;
	bool Do(RSData_Command& command, RSData_Map& map) const override;
	bool Is(const RSData_Slot* slot) const override;
	bool Activate();
};

