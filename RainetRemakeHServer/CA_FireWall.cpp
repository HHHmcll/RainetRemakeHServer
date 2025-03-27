#include "CA_FireWall.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

CA_FireWall::CA_FireWall() :
	Owner(nullptr) {}

CA_FireWall::CA_FireWall(RSData_Player* owner) :
	Owner(owner) {}

std::shared_ptr<RS_TerminalCard> CA_FireWall::CreateNewObject(RSData_Player* owner) const
{
	return std::shared_ptr<RS_TerminalCard>(new CA_FireWall(owner));
}

bool CA_FireWall::CanDo(const RSData_Command& command, const RSData_Map& map) const
{
	if (!map.CheckPlayerType(command.Player)) {
		return false;
	}

	const RSData_Player& playerRef = map.getPlayer(command.Player == EPlayerType::Player1);
	const CA_FireWall* card = playerRef.GetTerminal<CA_FireWall>();
	if (!card) {
		return false;
	}
	if(card->TrappedSlot){
		return true;
	}else{
		if(command.Data.Coordinate.row1 == 0 || command.Data.Coordinate.row1 == 7){
			if( command.Data.Coordinate.col1 == 3 || command.Data.Coordinate.col1 == 4){
				return false;
			}
		}
		const RSData_Slot* commandSlot = map.getPieceSlot(command.Data.Coordinate.row1, command.Data.Coordinate.col1);
		// slot on board;
		if (!commandSlot || !commandSlot->bOnBoard) {
			return false;
		}
		// is my piece
		if (commandSlot->Piece && commandSlot->Piece->Player->PlayerID != command.Player) {
			return false;
		}

		if(map.IsTerminal(EActionType::FireWall, commandSlot) == EPlayerType(1 - commandSlot->Piece->Player->PlayerID)){
			return false;
		}
	}

	return true;
}

bool CA_FireWall::Do(RSData_Command& command, RSData_Map& map) const
{
	RSData_Player& playerRef = map.getPlayer(command.Player == EPlayerType::Player1);
	CA_FireWall* card = playerRef.GetTerminal<CA_FireWall>();
	
	if(card->TrappedSlot){
		card->TrappedSlot = nullptr;
	}else{
		card->TrappedSlot = map.getPieceSlot(command.Data.Coordinate.row1, command.Data.Coordinate.col1);
	}

	return true;

}


bool CA_FireWall::Is(const RSData_Slot* slot) const
{
	return slot == TrappedSlot;
}

void CA_FireWall::WriteToBuffer(const bool ShouldHide, std::vector<uint8_t>& buffer, const RSData_Map& map) const
{
	buffer.push_back(EActionType::FireWall);
	buffer.push_back(Owner->PlayerID);
	if (TrappedSlot == nullptr) {
		buffer.push_back(0);
		buffer.push_back(0xFF);
	}
	else {
		buffer.push_back(0xAA);
		buffer.push_back(map.GetCoordFromSlot(TrappedSlot));
	}
}

const RS_CommandAction* GetStaticFireWall() {
	static CA_FireWall FireWallStatic = CA_FireWall();
	return &FireWallStatic;
}


RS_CommandActionCreateFunction createFireWallFunction = RS_CommandActionCreateFunction(EActionType::FireWall, &GetStaticFireWall);