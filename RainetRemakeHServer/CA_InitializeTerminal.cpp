#include "CA_InitializeTerminal.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"
#include "RS_IOManager.h"
#include "CA_InitializePieces.h"

#include <vector>

CA_InitializeTerminal::CA_InitializeTerminal() {}

CA_InitializeTerminal* GetStaticInitializeTerminal() {
	static CA_InitializeTerminal instance = CA_InitializeTerminal();
	return &instance;
}

const RS_CommandAction* CreateInitializeTerminal() {
	return GetStaticInitializeTerminal();
}

bool CA_InitializeTerminal::CanDo(const RSData_Command& command, const RSData_Map& map) const
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
	if (command.Data.TerminalSetup != map.MaxTerminals){
		return false;
	}
	std::vector<EActionType>* terminals = (std::vector<EActionType>*)command.Meta.get();
	bool Took[EActionType::Num - EActionType::Move - 1] = {0};
	for(EActionType terminal : *terminals){
		if(terminal >= EActionType::Num  || terminal <= EActionType::Move){
			return false;
		}
		int terminalID = terminal - EActionType::Move - 1;
		if(Took[terminalID]){
			return false;
		}
		Took[terminalID] = true;
	}
	return  true;
}

bool CA_InitializeTerminal::Do(RSData_Command& command, RSData_Map& map) const
{
	RSData_Player player = map.getPlayer(command.Player);
	player.Cards.clear();
	std::vector<EActionType>* terminals = static_cast<std::vector<EActionType>*>(command.Meta.get());

	for(EActionType terminal : *terminals){
		player.Cards[terminal] = dynamic_cast<const RS_TerminalCard*>(RS_CommandActionManager::GetStaticAction(terminal))->CreateNewObject(nullptr);
	}

	if (GetStaticInitializeTerminal()->Initalized() && RS_CommandActionManager::GetStaticAction<CA_InitializePieces>()->Initalized()) {
		// not yet initialized
		struct out{

		}*te = new out{};
		RS_IOManager::QueueOutput(reinterpret_cast<uint8_t*>(te) ,sizeof(out) );
   	}
   
	return true;
}

bool CA_InitializeTerminal::Initalized() const{
	return Initialized[0] && Initialized[1];
}

RS_CommandActionCreateFunction createInitializeTerminalFunction = RS_CommandActionCreateFunction(EActionType::InitializeTerminal, &CreateInitializeTerminal);