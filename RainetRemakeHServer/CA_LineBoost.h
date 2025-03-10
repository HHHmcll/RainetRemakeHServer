#pragma once
#include "RS_CommandActionManager.h"

struct RSData_Piece;

class CA_LineBoost :public RS_TerminalCard {
private:
	CA_LineBoost();
	friend const RS_CommandAction* GetStaticLineBoost();
    RSData_Piece* InstalledPiece;

public:
	static const EActionType StaticType = EActionType::LineBoost;
	std::shared_ptr<RS_TerminalCard> CreateNewObject(void* meta) const override;
	bool CanDo(RSData_Command& command, RSData_Map& map) const override;
	bool Do(RSData_Command& command, RSData_Map& map) const override;
	bool Is(const RSData_Slot* slot) const override;
};

