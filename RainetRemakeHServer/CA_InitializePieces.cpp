#include "CA_InitializePieces.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

#include "CA_InitializeTerminal.h"

CA_InitializePieces* GetStaticInitializePieces() {
	static CA_InitializePieces instance = CA_InitializePieces();
	return &instance;
}

const RS_CommandAction* GetStaticInitializePiecesConstWrapper() {
	return GetStaticInitializePieces();
}

CA_InitializePieces::CA_InitializePieces() {}

bool CA_InitializePieces::CanDo(RSData_Command& command, RSData_Map& map) const
{
	if (Initialized[command.Player]){
		return false;
	}
	if (map.GetGameState() != EGameState::Initialization){
		return false;
	}
	if (command.Player != (command.Player & 0x01)){
		return false;
	}
	return  true;
	
}

bool CA_InitializePieces::Do(RSData_Command& command, RSData_Map& map) const
{
	RSData_Player player = map.getPlayer(command.Player == EPlayerType::Player1);

	for (int i = 0; i < MAP_SIZE; i++)
	{
		player.pieces[i].Type = EPieceType(command.Data.PieceSetup.setup & 1 << i);
	}
	GetStaticInitializePieces()->Initialized[command.Player] = true;

	if(GetStaticInitializePieces()->Initalized() && RS_CommandActionManager::GetStaticAction<CA_InitializeTerminal>()->Initalized()){
 		// not yet initialized
	}

	return true;
}

bool CA_InitializePieces::Initalized() const{
	return Initialized[0] && Initialized[1];
}

RS_CommandActionCreateFunction createInitializePieceFunction = RS_CommandActionCreateFunction(EActionType::InitializePieces, &GetStaticInitializePiecesConstWrapper);