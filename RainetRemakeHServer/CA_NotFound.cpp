#include "CA_NotFound.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

CA_NotFound::CA_NotFound():used(false) {}

std::shared_ptr<RS_TerminalCard> CA_NotFound::CreateNewObject(void* meta) const
{
	return std::shared_ptr<RS_TerminalCard>(new CA_NotFound());
}


bool CA_NotFound::CanDo(const RSData_Command& command, const RSData_Map& map) const
{
	if (!map.CheckPlayerType(command.Player)) {
		return false;
	}

	const RSData_Player& playerRef = map.getPlayer(command.Player == EPlayerType::Player1);
	const CA_NotFound* card = playerRef.GetTerminal<CA_NotFound>();
	if (!card) {
		return false;
	}
	if (card->used) {
		return false;
	}

	const RSData_Slot* commandSlotFrom = map.getPieceSlot(abs(command.Data.Coordinate.row1), abs(command.Data.Coordinate.col1));
	if (!commandSlotFrom || !commandSlotFrom->bOnBoard) {
		return false;
	}

	const RSData_Slot* commandSlotTo = map.getPieceSlot(abs(command.Data.Coordinate.row2), abs(command.Data.Coordinate.col2));
	if (!commandSlotTo || !commandSlotTo->bOnBoard) {
		return false;
	}

	if (!commandSlotFrom->Piece || !commandSlotTo->Piece) {
		return false;
	}

	if (!commandSlotFrom->Piece->Player == command.Player|| !commandSlotTo->Piece->Player == command.Player) {
		return false;
	}

	if(map.IsTerminal(EActionType::SandBox, commandSlotFrom) != EPlayerType::Empty){
		return false;
	}

	if (map.IsTerminal(EActionType::SandBox, commandSlotTo) != EPlayerType::Empty) {
		return false;
	}

	return true;
}

bool CA_NotFound::Do(RSData_Command& command, RSData_Map& map) const
{
	RSData_Player& playerRef = map.getPlayer(command.Player == EPlayerType::Player1);
	CA_NotFound* card = playerRef.GetTerminal<CA_NotFound>();
	card->used = true;
	RSData_Slot* commandSlotFrom = map.getPieceSlot(abs(command.Data.Coordinate.row1), abs(command.Data.Coordinate.col1));
	RSData_Slot* commandSlotTo = map.getPieceSlot(abs(command.Data.Coordinate.row2), abs(command.Data.Coordinate.col2));
	commandSlotFrom->Piece->revealed = false;
	commandSlotTo->Piece->revealed = false;
	if (command.Data.Coordinate.row1 < 0 || command.Data.Coordinate.row2 < 0 || command.Data.Coordinate.col1 < 0 || command.Data.Coordinate.col2 < 0) {
		return true;
	}
	RSData_Piece* temp = commandSlotTo->Piece;
	commandSlotTo->Piece = commandSlotFrom->Piece;
	commandSlotFrom->Piece = temp;
	commandSlotFrom->Piece->Slot = commandSlotFrom;
	commandSlotTo->Piece->Slot = commandSlotTo;
	return true;

}


bool CA_NotFound::Is(const RSData_Slot* slot) const
{
	return false;
}

const RS_CommandAction* GetStaticNotFound() {
	static CA_NotFound NotFoundStatic = CA_NotFound();
	return &NotFoundStatic;
}


RS_CommandActionCreateFunction createNotFoundFunction = RS_CommandActionCreateFunction(EActionType::NotFound, &GetStaticNotFound);