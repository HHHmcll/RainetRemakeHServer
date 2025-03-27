#include "CA_VirusCheck.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

CA_VirusCheck::CA_VirusCheck() :
	used(false), Owner(nullptr) {}

CA_VirusCheck::CA_VirusCheck(RSData_Player* owner) :
	used(false),Owner(owner) {}

std::shared_ptr<RS_TerminalCard> CA_VirusCheck::CreateNewObject(RSData_Player* owner) const
{
	return std::shared_ptr<RS_TerminalCard>(new CA_VirusCheck(owner));
}


bool CA_VirusCheck::CanDo(const RSData_Command& command,const RSData_Map& map) const
{
	if (!map.CheckPlayerType(command.Player)) {
		return false;
	}

	const RSData_Player& playerRef = map.getPlayer(command.Player == EPlayerType::Player1);
	const CA_VirusCheck* card = playerRef.GetTerminal<CA_VirusCheck>();

	if (!card || card->used) {
		return false;
	}

	const RSData_Slot* commandSlot = map.getPieceSlot(command.Data.Coordinate.row1, command.Data.Coordinate.col1);
	if (!commandSlot || !commandSlot->bOnBoard) {
		return false;
	}
	if(map.IsTerminal(EActionType::SandBox, commandSlot) != EPlayerType::Empty){
		return false;
	}

	RSData_Piece* piece = commandSlot->Piece;
	if (!piece || piece->Player->PlayerID == command.Player) {
		return false;
	}

	return true;
}

bool CA_VirusCheck::Do(RSData_Command& command, RSData_Map& map) const
{
	CA_VirusCheck* card = map.getPlayer(command.Player == EPlayerType::Player1).GetTerminal<CA_VirusCheck>();

	card->used = true;

	map.getPiece(command.Data.Coordinate.row1, command.Data.Coordinate.col1)->revealed = true;

	return true;

}
bool CA_VirusCheck::Is(const RSData_Slot* slot) const
{
	return false;
}

void CA_VirusCheck::WriteToBuffer(const bool ShouldHide, std::vector<uint8_t>& buffer, const RSData_Map& map) const
{
	buffer.push_back(EActionType::VirusCheck);
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


const RS_CommandAction* GetStaticVirusCheck() {
	static CA_VirusCheck VirusCheckStatic = CA_VirusCheck();
	return &VirusCheckStatic;
}


RS_CommandActionCreateFunction createVirusCheckFunction = RS_CommandActionCreateFunction(EActionType::VirusCheck, &GetStaticVirusCheck);