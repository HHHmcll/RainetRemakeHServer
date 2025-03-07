#pragma once
#include "RS_CommandActionManager.h"

struct RSData_Piece;
struct RSData_Player;

class CA_Move :public RS_CommandAction {
private:
	CA_Move();
	friend RS_CommandAction* GetStaticMove();
    RSData_Piece* InstalledPiece;

public:
	std::shared_ptr<RS_CommandAction> CreateNewObject(void* meta) override;
	bool CanDo(RSData_Command& command, RSData_Map& map) override;
	bool Do(RSData_Command& command, RSData_Map& map) override;
	EBlock_Status Block(RSData_Player* owner, RSData_Command& command, RSData_Map& map) override;

};

