#include "CA_LineBoost.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

CA_LineBoost::CA_LineBoost() :
	InstalledPiece(nullptr) {}

std::shared_ptr<RS_TerminalCard> CA_LineBoost::CreateNewObject(void* meta) const
{
	return std::shared_ptr<RS_TerminalCard>(new CA_LineBoost());
}


bool CA_LineBoost::CanDo(const RSData_Command& command, const RSData_Map& map) const
{
	if (!map.CheckPlayerType(command.Player)) {
		return false;
	}

	const RSData_Player& playerRef = map.getPlayer(command.Player == EPlayerType::Player1);
	const CA_LineBoost* card = playerRef.GetTerminal<CA_LineBoost>();
	if (!card) {
		return false;
	}
	const RSData_Slot* commandSlot = map.getPieceSlot(command.Data.Coordinate.row1, command.Data.Coordinate.col1);
	if (!commandSlot) {
		return false;
	}
	if(map.IsTerminal(EPlayerType::Player1, EActionType::SandBox, commandSlot) || map.IsTerminal(EPlayerType::Player2, EActionType::SandBox, commandSlot)){
		return false;
	}

	 RSData_Piece* const& commandPiece = card->InstalledPiece;
	if (commandPiece) {

		if (commandSlot->Piece != commandPiece) {
			return false;
		}
	}
	else {
		RSData_Piece* piece = commandSlot->Piece;
		if (!piece || piece->Player->PlayerID != command.Player) {
			return false;
		}
	}
	return true;
}

bool CA_LineBoost::Do(RSData_Command& command, RSData_Map& map) const
{
	CA_LineBoost* card = map.getPlayer(command.Player == EPlayerType::Player1).GetTerminal<CA_LineBoost>();
	RSData_Piece*& commandPiece = card->InstalledPiece;

	if (commandPiece) {
		commandPiece = nullptr;
	}
	else {
		commandPiece = map.getPiece(command.Data.Coordinate.row1, command.Data.Coordinate.col1);
	}
	return true;

}


bool CA_LineBoost::Is(const RSData_Slot* slot) const
{
	return slot && InstalledPiece && slot->Piece == InstalledPiece;
}

const RS_CommandAction* GetStaticLineBoost() {
	static CA_LineBoost LineBoostStatic = CA_LineBoost();
	return &LineBoostStatic;
}


RS_CommandActionCreateFunction createLineBoostFunction = RS_CommandActionCreateFunction(EActionType::LineBoost, &GetStaticLineBoost);