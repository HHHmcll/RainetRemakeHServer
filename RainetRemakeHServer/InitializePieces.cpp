#include "InitializePieces.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

CA_InitializePieces::CA_InitializePieces() {

}

std::shared_ptr<RS_CommandAction> CA_InitializePieces::CreateNewObject(void* meta) 
{
	return std::shared_ptr<RS_CommandAction>(nullptr);
}

bool CA_InitializePieces::CanDo(RSData_Command& command, RSData_Map& map)
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

bool CA_InitializePieces::Do(RSData_Command& command, RSData_Map& map)
{
	RSData_Player player = map.getPlayer(command.Player == EPlayerType::Player1);

	for (int i = 0; i < MAP_SIZE; i++)
	{
		player.pieces[i].Type = EPieceType(command.Data.PieceSetup.setup & 1 << i);
	}
	Initialized[command.Player] = true;

	if(Block(nullptr, command,map) == EBlock_Status::Block || RS_CommandActionManager::GetStaticAction(EActionType::InitializeTerminal)->Block(nullptr, command,map) == EBlock_Status::Block){
 		// not yet initialized
	}

	return true;
}

EBlock_Status CA_InitializePieces::Block(RSData_Player* owner, RSData_Command& command, RSData_Map& map)
{
	for (bool initialized : Initialized) {
		if (initialized) return EBlock_Status::Pass;
	}
	return EBlock_Status::Block;
}


RS_CommandAction* GetStaticInitializePieces() {
	static CA_InitializePieces instance = CA_InitializePieces();
	return &instance;
}

RS_CommandActionCreateFunction createInitializePieceFunction = RS_CommandActionCreateFunction(EActionType::LineBoost, &GetStaticInitializePieces);