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

	if (commandSlotFrom == commandSlotTo) {
		return true;
	}
	
	if(commandSlotTo->bOnBoard){
		
		EPlayerType RabbitPlayer = map.IsTerminal(EActionType::RabbitTrap,commandSlotTo);
		if (RabbitPlayer != EPlayerType::Empty) {
			auto * RabbitTrap = map.getPlayer(RabbitPlayer == EPlayerType::Player1).GetTerminal<CA_RabbitTrap>();
			RabbitTrap->TrappedSlot = nullptr;
			
			commandSlotFrom->Piece->revealed = true;
			EPlayerType SandBoxPlayer = map.IsTerminal(EActionType::SandBox, commandSlotFrom);
			if (SandBoxPlayer != EPlayerType::Empty) {
				auto* sandBox = map.getPlayer(SandBoxPlayer == EPlayerType::Player1).GetTerminal<CA_SandBox>();
				sandBox->TrappedPiece = nullptr;
			}
			
		}
		
		if(commandSlotTo->Piece){
			playerRef.AteCount[commandSlotTo->Piece->Type] ++;
			playerRef.CaptureSlot[commandSlotTo->Piece->Type].push_back(commandSlotTo->Piece);
			commandSlotTo->Piece->OnPieceRemovedFromBoard.BroadCast(command);
			commandSlotTo->Piece->revealed = true;
			commandSlotTo->Piece->Slot = nullptr;
			commandSlotTo->Piece = nullptr;
		}
		
		commandSlotTo->Piece = commandSlotFrom->Piece;
		commandSlotTo->Piece->Slot = commandSlotTo;
		commandSlotFrom->Piece = nullptr;

	}else{
		auto& playerBeEntered = map.getPlayer(commandSlotTo->SlotID == EPlayerType::Player1);
		playerRef.EnterCount[commandSlotFrom->Piece->Type] ++;
		playerBeEntered.ServerSlots.push_back(commandSlotFrom->Piece);

		commandSlotFrom->Piece->OnPieceRemovedFromBoard.BroadCast(command);
		
		commandSlotFrom->Piece->Slot = nullptr;
		commandSlotFrom->Piece = nullptr;
	}
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