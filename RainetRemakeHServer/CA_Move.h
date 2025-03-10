#pragma once
#include "RS_CommandActionManager.h"

struct RSData_Piece;
struct RSData_Player;

class CA_Move :public RS_CommandAction {
private:
	CA_Move();
	friend const RS_CommandAction* GetStaticMove();
public:
	static const EActionType StaticType = EActionType::Move;
	bool CanDo(RSData_Command& command, RSData_Map& map) const override;
	bool Do(RSData_Command& command, RSData_Map& map) const override;
};

