#include "CA_RabbitTrap.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

CA_RabbitTrap::CA_RabbitTrap():TrappedSlot(nullptr), used(false){}

std::shared_ptr<RS_TerminalCard> CA_RabbitTrap::CreateNewObject(void* meta) const
{
	return std::shared_ptr<RS_TerminalCard>(new CA_RabbitTrap());
}


bool CA_RabbitTrap::CanDo(const RSData_Command& command, const RSData_Map& map) const
{
	if (!map.CheckPlayerType(command.Player)) {
		return false;
	}

	const RSData_Player& playerRef = map.getPlayer(command.Player == EPlayerType::Player1);
	const CA_RabbitTrap* card = playerRef.GetTerminal<CA_RabbitTrap>();
	if (!card) {
		return false;
	}
	if (card->used) {
		return false;
	}

	const RSData_Slot* commandSlot = map.getPieceSlot(command.Data.Coordinate.row1, command.Data.Coordinate.col1);
	// slot on board;
	if (!commandSlot) {
		return false;
	}
	// is my piece
	if (!commandSlot->Piece || commandSlot->Piece->Player->PlayerID != command.Player) {
		return false;
	}

	if(map.IsTerminal(EPlayerType(1 - commandSlot->Piece->Player->PlayerID), EActionType::RabbitTrap, commandSlot)){
		return false;
	}

	return true;
}

bool CA_RabbitTrap::Do(RSData_Command& command, RSData_Map& map) const
{
	RSData_Player& playerRef = map.getPlayer(command.Player == EPlayerType::Player1);
	CA_RabbitTrap* card = playerRef.GetTerminal<CA_RabbitTrap>();
	card->used = true;
	card->TrappedSlot = map.getPieceSlot(command.Data.Coordinate.row1, command.Data.Coordinate.col1);

	return true;

}


bool CA_RabbitTrap::Is(const RSData_Slot* slot) const
{
	return false;
}

const RS_CommandAction* GetStaticRabbitTrap() {
	static CA_RabbitTrap RabbitTrapStatic = CA_RabbitTrap();
	return &RabbitTrapStatic;
}


RS_CommandActionCreateFunction createRabbitTrapFunction = RS_CommandActionCreateFunction(EActionType::RabbitTrap, &GetStaticRabbitTrap);