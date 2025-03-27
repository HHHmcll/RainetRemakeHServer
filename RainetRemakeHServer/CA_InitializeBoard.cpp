#include "CA_InitializeBoard.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"
#include "RS_IOManager.h"


#include <vector>

CA_InitializeBoard::CA_InitializeBoard() {}

CA_InitializeBoard* GetStaticInitializeBoard() {
	static CA_InitializeBoard instance = CA_InitializeBoard();
	return &instance;
}

const RS_CommandAction* GetStaticInitializeBoardConstWrapper() {
	return GetStaticInitializeBoard();
}

bool CA_InitializeBoard::CanDo(const RSData_Command& command, const RSData_Map& map) const
{

	if (map.GetGameState() != EGameState::Initialization) {
		return false;
	}

	if (command.Data.TerminalSetup != (map.MaxTerminals * 2 + 8 * 2)) {
		return false;
	}

	std::vector<uint8_t>& SetupData = *((std::vector<uint8_t>*)command.Meta.get());
	int setupPTR = 0;
	for (auto& player : { map.getPlayer(true), map.getPlayer(false) }) {
		int L = 0, V = 0;
		for (int i = 0; i < 8; i++, setupPTR++) {
			if (SetupData[setupPTR] == 'L') {
				L++;
			}
			else if (SetupData[setupPTR] == 'V') {
				V++;
			}
			else {
				return false;
			}
		}

		if (L != 4 || V != 4) {
			return false;
		}

		bool Took[EActionType::Num - EActionType::Move - 1] = {0};

		for(size_t i = 0; i < map.MaxTerminals; i++, setupPTR++){
			if(SetupData[setupPTR] >= EActionType::Num  || SetupData[setupPTR] <= EActionType::Move){
				return false;
			}
			int terminalID = SetupData[setupPTR] - EActionType::Move - 1;
			if(Took[terminalID]){
				return false;
			}
			Took[terminalID] = true;
		}
	}
	return  true;
}

bool CA_InitializeBoard::Do(RSData_Command& command, RSData_Map& map) const
{

	std::vector<uint8_t>& SetupData = *((std::vector<uint8_t>*)command.Meta.get());
	int setupPTR = 0;
	for (bool isPlayer1 : {true, false}) {
		RSData_Player& player = map.getPlayer(isPlayer1);
		player.Cards.clear();
		for (int i = 0; i < 8; i++, setupPTR++) {
			if (SetupData[setupPTR] == 'L') {
				player.pieces->Type = EPieceType::Link;
			}
			else {
				player.pieces->Type = EPieceType::Virus;
			}
		}

		for (size_t i = 0; i < map.MaxTerminals; i++, setupPTR++) {
			EActionType termType = EActionType(SetupData[setupPTR]);
			player.Cards[termType] = dynamic_cast<const RS_TerminalCard*>(RS_CommandActionManager::GetStaticAction(termType))->CreateNewObject(&player);
		}
	}


	return true;
}



RS_CommandActionCreateFunction createInitializeBoardFunction = RS_CommandActionCreateFunction(EActionType::InitializeBoard, &GetStaticInitializeBoardConstWrapper);