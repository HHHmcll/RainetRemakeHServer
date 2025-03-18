#include "CA_Move.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"
#include "CA_SandBox.h"
#include "CA_RabbitTrap.h"

CA_Move::CA_Move(){}

bool CA_Move::CanDo(const RSData_Command& command, const RSData_Map& map) const
{
    
	if (!map.CheckPlayerType(command.Player)) {
		return false;
	}

	const RSData_Player& playerRef = map.getPlayer(command.Player == EPlayerType::Player1);
	auto& Coordinate = command.Data.Coordinate;
	const RSData_Slot* commandSlotFrom = map.getPieceSlot(Coordinate.row1, Coordinate.col1);

	if (!commandSlotFrom || !commandSlotFrom->bOnBoard) {
		return false;
	}
	if(!commandSlotFrom->Piece){
		return false;
	}
	if( command.Player != commandSlotFrom->Piece->Player->PlayerID){
		return false;
	}
	const RSData_Slot* commandSlotTo = map.getPieceSlot(Coordinate.row2, Coordinate.col2);

	if (!commandSlotTo) {
		return false;
	}

	if(!commandSlotTo->bOnBoard){
		if(command.Player == commandSlotTo->SlotID){
			return false;
		}
		if(Coordinate.row1 >= 2 && Coordinate.row1 <= 5){
			return false;
		}
		
		if(Coordinate.col1 < 2 || Coordinate.col1 > 5){
				return false;
		}
		if(Coordinate.col1 == 2 || Coordinate.col1 == 5){
			if(Coordinate.row1 != 7 || Coordinate.row1 != 0){
				return false;
			}
		}
		if(Coordinate.col1 == 3 || Coordinate.col1 == 4){
			if(Coordinate.row1 == 7 && commandSlotTo->SlotID == EPlayerType::Player2){
				return true;
			}else if(Coordinate.row1 == 0 && commandSlotTo->SlotID == EPlayerType::Player1){
				return true;
			}
		}
		if(map.IsTerminal(EActionType::LineBoost, commandSlotFrom) != command.Player && map.IsTerminal(EActionType::SandBox,commandSlotFrom) == EPlayerType::Empty){
			return false;
		}

		const RSData_Slot* SlotPass = map.getPieceSlot(commandSlotTo->SlotID == EPlayerType::Player1 ? 0 : 7, Coordinate.col1); 
		if(map.IsTerminal(EActionType::FireWall,SlotPass) == EPlayerType(1-command.Player)){
			return false;
		}else if(map.IsTerminal(EActionType::RabbitTrap,SlotPass) != EPlayerType::Empty){
			return false;
		}
		return true;
	}
    
	if(map.IsTerminal(EActionType::FireWall, commandSlotTo) == EPlayerType(1 - command.Player)){
        return false;
    }

	if(commandSlotTo->Piece && commandSlotTo->Piece->Player->PlayerID == command.Player){
		return false;
	}

	int moveDist = abs(Coordinate.col1-Coordinate.col2) + abs(Coordinate.row1 - Coordinate.row2);

	if(moveDist == 1){
		return true;
	}else if(moveDist == 2){
		if(map.IsTerminal(EActionType::LineBoost,commandSlotFrom) != command.Player && !(map.IsTerminal(EActionType::SandBox, commandSlotFrom) != EPlayerType::Empty && !commandSlotFrom->Piece->revealed)){
			return false;
		}
		const RSData_Slot*  SlotPassCol = nullptr;
		const RSData_Slot*  SlotPassRow = nullptr;
		if(Coordinate.col1 != Coordinate.col2){
			
			SlotPassCol = map.getPieceSlot(Coordinate.row1, Coordinate.col1 > Coordinate.col2 ? Coordinate.col1 - 1 : Coordinate.col1 + 1); 
			
			if(map.IsTerminal(EActionType::RabbitTrap,SlotPassCol) != EPlayerType::Empty){
				return false;
			}
		}
		if(Coordinate.row1 != Coordinate.row2){
			SlotPassRow = map.getPieceSlot(Coordinate.row1 > Coordinate.row2 ? Coordinate.row1 - 1 : Coordinate.row1 + 1, Coordinate.col1); 
			if(map.IsTerminal(EActionType::RabbitTrap,SlotPassRow) != EPlayerType::Empty){
				return false;
			}
		}
		if(SlotPassCol == nullptr && map.IsTerminal(EActionType::FireWall,SlotPassRow) == EPlayerType(1-command.Player)){
			return false;
		}
		if(SlotPassRow == nullptr && map.IsTerminal(EActionType::FireWall,SlotPassCol) == EPlayerType(1-command.Player)){
			return false;
		}
		return true;
	}else{
		return false;
	}
	
}
bool CA_Move::Do(RSData_Command& command, RSData_Map& map) const
{

	RSData_Player& playerRef = map.getPlayer(command.Player == EPlayerType::Player1);
	auto& Coordinate = command.Data.Coordinate;
	RSData_Slot* commandSlotFrom = map.getPieceSlot(Coordinate.row1, Coordinate.col1);
	RSData_Slot* commandSlotTo = map.getPieceSlot(Coordinate.row2, Coordinate.col2);


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


const RS_CommandAction* GetStaticMove() {
    static CA_Move MoveStatic = CA_Move();
    return &MoveStatic;
}


RS_CommandActionCreateFunction createMoveFunction = RS_CommandActionCreateFunction(EActionType::Move, &GetStaticMove);