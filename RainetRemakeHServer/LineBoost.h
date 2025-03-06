#pragma once
#include "RS_CommandActionManager.h"

struct RSData_Piece;

class CA_LineBoost :public RS_CommandAction {
private:
	CA_LineBoost();
	friend RS_CommandAction* GetStaticLineBoost();
    RSData_Piece* installedPiece;
public:
	std::shared_ptr<RS_CommandAction> CreateNewObject(void* meta) override;
	bool CanDo(RSData_Command& command, RSData_Map& map) override;
	bool Do(RSData_Command& command, RSData_Map& map) override;
	bool Block(RSData_Command& command, RSData_Map& map) override;
};

