#include "CA_RabbitTrap.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

CA_RabbitTrap::CA_RabbitTrap() :
	used(false), Owner(nullptr) {}

CA_RabbitTrap::CA_RabbitTrap(RSData_Player* owner) :
	used(false), Owner(owner) {}

std::shared_ptr<RS_TerminalCard> CA_RabbitTrap::CreateNewObject(RSData_Player* owner) const
{
	return std::shared_ptr<RS_TerminalCard>(new CA_RabbitTrap(owner));
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
	if (!commandSlot || !commandSlot->bOnBoard) {
		return false;
	}
	// is my piece
	if (!commandSlot->Piece || commandSlot->Piece->Player->PlayerID != command.Player) {
		return false;
	}

	if(map.IsTerminal(EActionType::RabbitTrap, commandSlot) == EPlayerType(1 - commandSlot->Piece->Player->PlayerID)){
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
	for (RSData_Piece& piece : playerRef.pieces) {
		if (piece.Slot->bOnBoard && piece.Slot != card->TrappedSlot) {
			card->fakeSlots.push_back(piece.Slot);
		}
	}
	map.MapPreMoveDelegate.Add(EActionType::RabbitTrap, [&](RSData_Command& command) {
		card->fakeSlots.erase(std::find(card->fakeSlots.begin(),card->fakeSlots.end(),map.getPieceSlot(command.Data.Coordinate.row2, command.Data.Coordinate.col2)));
		if (card->fakeSlots.empty()) {
			map.MapPreMoveDelegate.Remove(EActionType::RabbitTrap);
		}
	});
	return true;
}


bool CA_RabbitTrap::Is(const RSData_Slot* slot) const
{
	return TrappedSlot == slot;
}

void CA_RabbitTrap::WriteToBuffer(const bool ShouldHide, std::vector<uint8_t>& buffer, const RSData_Map& map) const
{
	buffer.push_back(EActionType::RabbitTrap);
	buffer.push_back(Owner->PlayerID);
	if (used) {
		buffer.push_back(0xAA);
		buffer.push_back(map.GetCoordFromSlot(TrappedSlot));
		for (RSData_Slot* fakeSlot: fakeSlots)
		{
			buffer.push_back(EActionType::RabbitTrap);
			buffer.push_back(Owner->PlayerID);
			buffer.push_back(ShouldHide ? 0xAA : 0xA0);
			buffer.push_back(map.GetCoordFromSlot(fakeSlot));
		}
	}
	else {
		buffer.push_back(0);
		buffer.push_back(0);
	}
}

const RS_CommandAction* GetStaticRabbitTrap() {
	static CA_RabbitTrap RabbitTrapStatic = CA_RabbitTrap();
	return &RabbitTrapStatic;
}


RS_CommandActionCreateFunction createRabbitTrapFunction = RS_CommandActionCreateFunction(EActionType::RabbitTrap, &GetStaticRabbitTrap);