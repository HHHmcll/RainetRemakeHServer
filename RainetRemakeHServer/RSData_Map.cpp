#include "RSData_Map.h"

#include "RS_CommandActionManager.h"

RSData_Piece::RSData_Piece(RSData_Player* player, EPieceType type):
	Player(player), Type(type), Slot(nullptr)
{}

RSData_Player::RSData_Player(uint8_t playerID) :
	PlayerID(playerID), LinkAte(0), LinkEnter(0), VirusAte(0), VirusEnter(0),
	pieces{ RSData_Piece(this,EPieceType::Unknown), RSData_Piece(this,EPieceType::Unknown),
		RSData_Piece(this,EPieceType::Unknown), RSData_Piece(this,EPieceType::Unknown),
		RSData_Piece(this,EPieceType::Unknown), RSData_Piece(this,EPieceType::Unknown),
		RSData_Piece(this,EPieceType::Unknown), RSData_Piece(this,EPieceType::Unknown) }
{}

bool RSData_Player::ForEachTerminal(std::function<bool(RS_CommandAction*)> callback)
{
	for (auto terminal : Cards)
	{
		if (callback(terminal.second.get())) {
			return true;
		}
	}
	return false;
}

RSData_Map::RSData_Map(uint32_t maxTerminals)
	:playerData{RSData_Player(0),RSData_Player(1)},
	board{0}, 
	gameState(EGameState::Initialization),
	MaxTerminals(maxTerminals)
{

	const static constexpr int player1Coord[8] = {
		CoordToID(0,0),
		CoordToID(0,1),
		CoordToID(0,2),
		CoordToID(1,3),
		CoordToID(1,4),
		CoordToID(0,5),
		CoordToID(0,6),
		CoordToID(0,7),
	};
	const static constexpr int player2Coord[8] = {
		CoordToID(7,0),
		CoordToID(7,1),
		CoordToID(7,2),
		CoordToID(6,3),
		CoordToID(6,4),
		CoordToID(7,5),
		CoordToID(7,6),
		CoordToID(7,7),
	};

	for (int i = 0; i < MAP_SIZE; i++) {
		board[player1Coord[i]] = &playerData[0].pieces[i];
		playerData[0].pieces[i].Slot = &board[player1Coord[i]];
		board[player2Coord[i]] = &playerData[1].pieces[i];
		playerData[1].pieces[i].Slot = &board[player1Coord[i]];
	}
}


RSData_Piece* RSData_Map::getPiece(uint8_t row, uint8_t col)
{
	return *getPieceSlot(row,col);
}
RSData_Piece** RSData_Map::getPieceSlot (uint8_t row, uint8_t col)
{
	if (row < 0 || row >= MAP_SIZE) return nullptr;
	if (col < 0 || col >= MAP_SIZE) return nullptr;
	return &board[CoordToID(row,col)];
}

RSData_Player& RSData_Map::getPlayer(bool isPlayer1)
{
	return playerData[!isPlayer1] ;
}

bool RSData_Map::ForEachPlayer(std::function<bool(RSData_Player*)> callback)
{
	for (RSData_Player& player : playerData)
	{
		if (callback(&player)) {
			return true;
		}
	}
	return false;
}

const EGameState RSData_Map::GetGameState() const{
	return gameState;
}

void RSData_Map::SetGameState(EGameState newState){
	gameState = newState;
}

bool RSData_Map::GetCoordFromSlot(RSData_Piece** slot, uint8_t& row, uint8_t& col)
{
	int id = board - slot;

	if (id < 0 || id >= MAP_SIZE * MAP_SIZE) return false;
	row = id / MAP_SIZE;
	col = id % MAP_SIZE;
	return true;
}
