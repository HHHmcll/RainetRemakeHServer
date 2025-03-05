#include "InitializeTerminal.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"
#include <vector>
std::shared_ptr<RS_CommandAction> CA_InitializeTerminal::CreateNewObject(void* meta)
{
	return std::shared_ptr<RS_CommandAction>(nullptr);
}

bool CA_InitializeTerminal::CanDo(RSData_Command& command, RSData_Map& map)
{
	if (map.GetGameState() != EGameState::Initialization){
		return false;
	}
	if (command.Player != command.Player & 0x01){
		return false;
	}
	if (command.Data.TerminalSetup != map.MaxTerminals){
		return false;
	}
	std::vector<EActionType>* terminals = (std::vector<EActionType>*)command.Meta.get();
	for(EActionType terminal : *terminals){
		if(terminal > EActionType::Num  || terminal < EActionType::Move){
			return false;
		}
	}
	
	return  true;
	
}

bool CA_InitializeTerminal::Do(RSData_Command& command, RSData_Map& map, std::vector<uint8_t>& outputBuffer)
{
	RSData_Player player = map.getPlayer(command.Player == EPlayerType::Player1);

	
	
	return true;
}

bool CA_InitializeTerminal::Block(RSData_Command& command, RSData_Map& map)
{
	return false;
}

RS_CommandAction* CreateInitializeTerminal() {
	static CA_InitializeTerminal instance = CA_InitializeTerminal();
	return &instance;
}




RS_CommandActionCreateFunction createInitializeTerminalFunction = RS_CommandActionCreateFunction(EActionType::LineBoost, &CreateInitializeTerminal);