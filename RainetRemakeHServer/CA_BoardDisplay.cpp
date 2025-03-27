#include "CA_BoardDisplay.h"
#include "RSData_Map.h"
#include "RSData_Command.h"
#include "RS_IOManager.h"
#include <chrono>

CA_BoardDisplay::CA_BoardDisplay() : engine(std::chrono::system_clock::now().time_since_epoch().count()) {}

CA_BoardDisplay* GetBoardDisplay()
{
	static CA_BoardDisplay instance = CA_BoardDisplay();
	return &instance;
}

const RS_CommandAction* GetStaticBoardDisplay()
{
	return GetBoardDisplay();
}

bool CA_BoardDisplay::CanDo(const RSData_Command& command, const RSData_Map& map) const
{
	return true;
}

// 4 bytes for 1 line
// read lines to reconstruct 

bool CA_BoardDisplay::Do(RSData_Command& command, RSData_Map& map) const
{

	std::vector<uint8_t> data;
	map.ForEachPlayer([&](RSData_Player* player) -> bool {
		const bool show = player->PlayerID == command.Player || command.Player == EPlayerType::Everyone;
		for (const RSData_Piece& piece : player->pieces) {
			data.push_back(uint8_t(piece.Player->PlayerID));
			if (show || piece.revealed) {
				data.push_back(uint8_t(piece.Type));
			}
			else {
				data.push_back(uint8_t(EPieceType::Unknown));
			}

			data.push_back(uint8_t(piece.Slot->bOnBoard));
			data.push_back(map.GetCoordFromSlot(piece.Slot));
		}
		player->ForEachTerminal([&](RS_TerminalCard* terminal) -> bool {
			terminal->WriteToBuffer(show, data, map);
			return false;
		});
		return false;
	});

	uint8_t* output = new uint8_t[data.size() + 3];
	output[0] = command.Player;
	output[1] = EActionType::BoardDisplay;
	output[2] = uint8_t(data.size() / 4);

	memcpy(output + 3, data.data(), data.size());
	uint32_t* dataShuffle = reinterpret_cast<uint32_t*>(output + 3);
	std::shuffle(&dataShuffle[0], &dataShuffle[output[2]], GetBoardDisplay()->engine);
	RS_IOManager::QueueOutput(output, data.size() + 3);
	return false;
}


RS_CommandActionCreateFunction createBoardDisplayFunction = RS_CommandActionCreateFunction(EActionType::BoardDisplay, &GetStaticBoardDisplay);