#include "CA_LineBoost.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

CA_LineBoost::CA_LineBoost() :
	Owner(nullptr) {}

CA_LineBoost::CA_LineBoost(RSData_Player* owner) :
	Owner(owner) {}

std::shared_ptr<RS_TerminalCard> CA_LineBoost::CreateNewObject(RSData_Player* owner) const
{
	return std::shared_ptr<RS_TerminalCard>(new CA_LineBoost(owner));
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
	if (!commandSlot || !commandSlot->bOnBoard) {
		return false;
	}
	if(map.IsTerminal(EActionType::SandBox, commandSlot) != EPlayerType::Empty){
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
	
	commandPiece->OnPieceRemovedFromBoard.Add(StaticType,[&](RSData_Command& command){
		commandPiece = nullptr;
	});
	return true;

}


bool CA_LineBoost::Is(const RSData_Slot* slot) const
{
	return slot && InstalledPiece && slot->Piece == InstalledPiece;
}

void CA_LineBoost::WriteToBuffer(const bool ShouldHide, std::vector<uint8_t>& buffer, const RSData_Map& map) const
{
	buffer.push_back(EActionType::LineBoost);
	buffer.push_back(Owner->PlayerID);
	if (InstalledPiece == nullptr) {
		buffer.push_back(0x00);
		buffer.push_back(0xFF);
	}
	else {
		buffer.push_back(0xAA);
		buffer.push_back(map.GetCoordFromSlot(InstalledPiece->Slot));
	}
}

const RS_CommandAction* GetStaticLineBoost() {
	static CA_LineBoost LineBoostStatic = CA_LineBoost();
	return &LineBoostStatic;
}


RS_CommandActionCreateFunction createLineBoostFunction = RS_CommandActionCreateFunction(EActionType::LineBoost, &GetStaticLineBoost);