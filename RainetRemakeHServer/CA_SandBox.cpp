#include "CA_SandBox.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

CA_SandBox::CA_SandBox():TrappedSlot(nullptr), used(false){}

std::shared_ptr<RS_TerminalCard> CA_SandBox::CreateNewObject(void* meta) const
{
	return std::shared_ptr<RS_TerminalCard>(new CA_SandBox());
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
	if (!commandSlot) {
		return false;
	}
	// is my piece
	if (!commandSlot->Piece || commandSlot->Piece->Player->PlayerID != command.Player) {
		return false;
	}

	if(map.IsTerminal(EPlayerType(1 - commandSlot->Piece->Player->PlayerID), EActionType::SandBox, commandSlot)){
		return false;
	}

	return true;
}

bool CA_SandBox::Do(RSData_Command& command, RSData_Map& map) const
{
	RSData_Player& playerRef = map.getPlayer(command.Player == EPlayerType::Player1);
	CA_SandBox* card = playerRef.GetTerminal<CA_SandBox>();
	card->used = true;
	card->TrappedSlot = map.getPieceSlot(command.Data.Coordinate.row1, command.Data.Coordinate.col1);

	return true;

}


bool CA_SandBox::Is(const RSData_Slot* slot) const
{
	return false;
}

const RS_CommandAction* GetStaticSandBox() {
	static CA_SandBox SandBoxStatic = CA_SandBox();
	return &SandBoxStatic;
}


RS_CommandActionCreateFunction createSandBoxFunction = RS_CommandActionCreateFunction(EActionType::SandBox, &GetStaticSandBox);