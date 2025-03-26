#include "CA_BoardDisplay.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

const RS_CommandAction* GetStaticGetOutput()
{
	static CA_BoardDisplay instance = CA_BoardDisplay();
	return &instance;
}

bool CA_BoardDisplay::CanDo(const RSData_Command& command, const RSData_Map& map) const
{
	return true;
}

bool CA_BoardDisplay::Do(RSData_Command& command, RSData_Map& map) const
{

	std::vector<uint8_t> data;

	map.ForEachPlayer([&](RSData_Player* player) ->bool {
		const bool show = player->PlayerID == command.Player || command.Player == EPlayerType::Everyone;
		for (const RSData_Piece& piece : player->pieces) {
			data.push_back(uint8_t(piece.Player));
			if (show) {
				data.push_back(uint8_t(piece.Type));
			}
			else {
				data.push_back(uint8_t(EPieceType::Unknown));
			}

		}
		player->ForEachTerminal([&](RS_TerminalCard* terminal ) ->bool{
			
		});
		return false;
	})
	return false;
}


RS_CommandActionCreateFunction createGetOutputFunction = RS_CommandActionCreateFunction(EActionType::GetOutput, &GetStaticGetOutput);