#pragma once
#include "RS_CommandActionManager.h"

struct RSData_Piece;

class CA_NotFound :public RS_TerminalCard {
private:
	CA_NotFound();
	friend const RS_CommandAction* GetStaticNotFound();
	bool used;


public:
	static const EActionType StaticType = EActionType::NotFound;
	std::shared_ptr<RS_TerminalCard> CreateNewObject(void* meta) const override;
	bool CanDo(const RSData_Command& command, const RSData_Map& map) const override;
	bool Do(RSData_Command& command, RSData_Map& map) const override;
	bool Is(const RSData_Slot* slot) const override;
};

