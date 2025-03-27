#include "CA_SandBox.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

CA_SandBox::CA_SandBox() :
	used(false), Owner(nullptr) {}

CA_SandBox::CA_SandBox(RSData_Player* owner) :
	used(false), Owner(owner) {}

std::shared_ptr<RS_TerminalCard> CA_SandBox::CreateNewObject(RSData_Player* owner) const
{
	return std::shared_ptr<RS_TerminalCard>(new CA_SandBox(owner));
}



bool CA_SandBox::CanDo(const RSData_Command& command, const RSData_Map& map) const
{
	if (!map.CheckPlayerType(command.Player)) {
		return false;
	}

	const RSData_Player& playerRef = map.getPlayer(command.Player == EPlayerType::Player1);
	const CA_SandBox* card = playerRef.GetTerminal<CA_SandBox>();
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

	if(map.IsTerminal(EActionType::SandBox, commandSlot)!= EPlayerType::Empty){
		return false;
	}

	return true;
}

bool CA_SandBox::Do(RSData_Command& command, RSData_Map& map) const
{
	RSData_Player& playerRef = map.getPlayer(command.Player == EPlayerType::Player1);
	CA_SandBox* card = playerRef.GetTerminal<CA_SandBox>();
	card->used = true;
	card->TrappedPiece = map.getPiece(command.Data.Coordinate.row1, command.Data.Coordinate.col1);

	card->TrappedPiece->OnPieceRemovedFromBoard.Add(StaticType,[&](RSData_Command& command){
		card->TrappedPiece = nullptr;
	});
	return true;

}

bool CA_SandBox::Is(const RSData_Slot* slot) const
{
	return false;
}

void CA_SandBox::WriteToBuffer(const bool ShouldHide, std::vector<uint8_t>& buffer, const RSData_Map& map) const
{
	buffer.push_back(EActionType::SandBox);
	buffer.push_back(Owner->PlayerID);
	if (used) {
		buffer.push_back(0xAA);
		buffer.push_back(map.GetCoordFromSlot(TrappedPiece->Slot));
	}
	else {
		buffer.push_back(0);
		buffer.push_back(0);
	}
}


const RS_CommandAction* GetStaticSandBox() {
	static CA_SandBox SandBoxStatic = CA_SandBox();
	return &SandBoxStatic;
}


RS_CommandActionCreateFunction createSandBoxFunction = RS_CommandActionCreateFunction(EActionType::SandBox, &GetStaticSandBox);