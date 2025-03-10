#pragma once
#include "RS_CommandActionManager.h"

class CA_InitializePieces :public RS_CommandAction {
private:

	bool Initialized[2] = {0};
	CA_InitializePieces();
	friend CA_InitializePieces* GetStaticInitializePieces();
public:
	static const EActionType StaticType = EActionType::InitializePieces;
	bool CanDo(RSData_Command& command, RSData_Map& map) const override;
	bool Do(RSData_Command& command, RSData_Map& map) const override;
	bool Initalized() const ;
};
