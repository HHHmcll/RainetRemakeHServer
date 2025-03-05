#include "RSData_Map.h"


RSData_Map::RSData_Map(uint8_t maxTerminals)
	:playerData{RSData_Player(0),RSData_Player(1)},
	board(64,nullptr),
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
		board[player2Coord[i]] = &playerData[1].pieces[i];
	}
}

RSData_Piece* RSData_Map::getPiece(uint8_t row, uint8_t col)
{
	if (row < 0 || row >= MAP_SIZE) return nullptr;
	if (col < 0 || col >= MAP_SIZE) return nullptr;
	return board[CoordToID(row,col)];
}

inline RSData_Player& RSData_Map::getPlayer(bool isPlayer1)
{
	return playerData[!isPlayer1] ;
}
inline const EGameState RSData_Map::GetGameState() const{
	return gameState;
}

inline void RSData_Map::SetGameState(EGameState newState){
	gameState = newState;
}