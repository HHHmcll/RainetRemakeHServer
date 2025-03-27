#include "CA_ZeroDayAttack.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "CA_RabbitTrap.h"
#include "RSData_Command.h"
#include "CA_SandBox.h"

CA_ZeroDayAttack::CA_ZeroDayAttack() :
	used(false), Owner(nullptr) {}

CA_ZeroDayAttack::CA_ZeroDayAttack(RSData_Player* owner) :
	used(false), Owner(owner) {}

std::shared_ptr<RS_TerminalCard> CA_ZeroDayAttack::CreateNewObject(RSData_Player* owner) const
{
	return std::shared_ptr<RS_TerminalCard>(new CA_ZeroDayAttack(owner));
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

	
	const RSData_Slot* commandSlotFrom = map.getPieceSlot(command.Data.Coordinate.row1, command.Data.Coordinate.col1);
	if (!commandSlotFrom || !commandSlotFrom->bOnBoard) {
		return false;
	}

	const RSData_Slot* commandSlotEnd;
	const int offset = command.Player == Player1 ? 1 : -1;
	for (int i = 1; i < 4; i++) {
		
		commandSlotEnd = map.getPieceSlot(command.Data.Coordinate.row1 + offset * i, command.Data.Coordinate.col1);
		if(!commandSlotEnd){
			commandSlotEnd = map.getPieceSlot(command.Data.Coordinate.row1 + offset * (i-1), command.Data.Coordinate.col1 );	
		}
		if(!commandSlotEnd->bOnBoard){
			break;
		}
		if (map.IsTerminal(EActionType::RabbitTrap, commandSlotEnd) != EPlayerType::Empty) {
			break;
		}
		if (commandSlotEnd->Piece) {
			if (commandSlotEnd->Piece->Player->PlayerID == command.Player) {
				commandSlotEnd = map.getPieceSlot(command.Data.Coordinate.row1 + offset * (i-1), command.Data.Coordinate.col1);
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

	map.PerformMove(playerRef, command, commandSlotFrom, commandSlotTo);

	return true;
}


bool CA_ZeroDayAttack::Is(const RSData_Slot* slot) const
{
	return false;
}

void CA_ZeroDayAttack::WriteToBuffer(const bool ShouldHide, std::vector<uint8_t>& buffer, const RSData_Map& map) const
{
	buffer.push_back(EActionType::ZeroDayAttack);
	buffer.push_back(Owner->PlayerID);
	if (used) {
		buffer.push_back(0xAA);
		buffer.push_back(0xAA);
	}
	else {
		buffer.push_back(0);
		buffer.push_back(0);
	}
}

const RS_CommandAction* GetStaticZeroDayAttack() {
	static CA_ZeroDayAttack ZeroDayAttackStatic = CA_ZeroDayAttack();
	return &ZeroDayAttackStatic;
}


RS_CommandActionCreateFunction createZeroDayAttackFunction = RS_CommandActionCreateFunction(EActionType::ZeroDayAttack, &GetStaticZeroDayAttack);