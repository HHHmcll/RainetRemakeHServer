#include "CA_ZeroDayAttack.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "CA_RabbitTrap.h"
#include "RSData_Command.h"
#include "CA_SandBox.h"

CA_ZeroDayAttack::CA_ZeroDayAttack():used(false) {}

std::shared_ptr<RS_TerminalCard> CA_ZeroDayAttack::CreateNewObject(void* meta) const
{
	return std::shared_ptr<RS_TerminalCard>(new CA_ZeroDayAttack());
}



bool CA_ZeroDayAttack::CanDo(const RSData_Command& command, const RSData_Map& map) const
{
	if (!map.CheckPlayerType(command.Player)) {
		return false;
	}

	const RSData_Player& playerRef = map.getPlayer(command.Player == EPlayerType::Player1);
	const CA_ZeroDayAttack* card = playerRef.GetTerminal<CA_ZeroDayAttack>();
	if (!card) {
		return false;
	}
	if (card->used) {
		return false;
	}

	if (command.Data.Coordinate.col1 != command.Data.Coordinate.col2) {
		return false;
	}

	if (command.Data.Coordinate.row2 < command.Data.Coordinate.row1) {
		return false;
	}

	const RSData_Slot* commandSlotFrom = map.getPieceSlot(command.Data.Coordinate.row1, command.Data.Coordinate.col1);
	if (!commandSlotFrom) {
		return false;
	}

	const RSData_Slot* commandSlotEnd;
	for (int i = 1; i < 4; i++) {
		commandSlotEnd = map.getPieceSlot(command.Data.Coordinate.row1, command.Data.Coordinate.col1 + i);
		if (map.IsTerminal(EPlayerType::Player1, EActionType::RabbitTrap, commandSlotEnd) || map.IsTerminal(EPlayerType::Player2, EActionType::RabbitTrap, commandSlotEnd)) {
			break;
		}
		if (commandSlotEnd->Piece) {
			if (commandSlotEnd->Piece->Player->PlayerID == command.Player) {
				commandSlotEnd = map.getPieceSlot(command.Data.Coordinate.row1, command.Data.Coordinate.col1 + i - 1);
			}
			break;
		}
	}
	if (commandSlotEnd != map.getPieceSlot(command.Data.Coordinate.row2, command.Data.Coordinate.col2)) {
		return false;
	}

	return true;
}

bool CA_ZeroDayAttack::Do(RSData_Command& command, RSData_Map& map) const
{
	RSData_Player& playerRef = map.getPlayer(command.Player == EPlayerType::Player1);
	CA_ZeroDayAttack* card = playerRef.GetTerminal<CA_ZeroDayAttack>();
	card->used = true;


	RSData_Slot* commandSlotFrom = map.getPieceSlot(command.Data.Coordinate.row1, command.Data.Coordinate.col1);
	RSData_Slot* commandSlotTo = map.getPieceSlot(command.Data.Coordinate.row2, command.Data.Coordinate.col2);

	if (commandSlotFrom == commandSlotTo) {
		return true;
	}

	bool ShouldClearSandBox = false;

	if (map.IsTerminal(EPlayerType::Player1, EActionType::RabbitTrap,commandSlotTo)) {
		auto * RabbitTrap = map.getPlayer(true).GetTerminal<CA_RabbitTrap>();
		RabbitTrap->TrappedSlot = nullptr;
		ShouldClearSandBox = true;
	}

	if (map.IsTerminal(EPlayerType::Player2, EActionType::RabbitTrap, commandSlotTo)) {
		auto* RabbitTrap = map.getPlayer(false).GetTerminal<CA_RabbitTrap>();
		RabbitTrap->TrappedSlot = nullptr;
		ShouldClearSandBox = true;
	}
	if (ShouldClearSandBox) {

		if (map.IsTerminal(EPlayerType::Player1, EActionType::SandBox, commandSlotFrom)) {
			auto* sandBox = map.getPlayer(true).GetTerminal<CA_SandBox>();
			sandBox->TrappedSlot = nullptr;
		}
		else if (map.IsTerminal(EPlayerType::Player2, EActionType::SandBox, commandSlotFrom)) {
			auto* sandBox = map.getPlayer(false).GetTerminal<CA_SandBox>();
			sandBox->TrappedSlot = nullptr;
		}
	}

	RSData_Piece* temp = commandSlotTo->Piece;
	commandSlotTo->Piece = commandSlotFrom->Piece;
	commandSlotFrom->Piece = temp;

	return true;

}


bool CA_ZeroDayAttack::Is(const RSData_Slot* slot) const
{
	return false;
}

const RS_CommandAction* GetStaticZeroDayAttack() {
	static CA_ZeroDayAttack ZeroDayAttackStatic = CA_ZeroDayAttack();
	return &ZeroDayAttackStatic;
}


RS_CommandActionCreateFunction createZeroDayAttackFunction = RS_CommandActionCreateFunction(EActionType::ZeroDayAttack, &GetStaticZeroDayAttack);