#pragma once

#include "Enums.h"
#include <map>
#include <vector>
#define MAP_SIZE (8)
struct RSData_Piece
{
    EPlayerType Player;
    EPawnType Type;
    RSData_Piece(EPlayerType player, EPawnType type):Player(player), Type(type){}
};

class RS_CommandAction;

struct RSData_Player
{
    uint8_t LinkAte, LinkEnter;
    uint8_t VirusAte, VirusEnter;
    std::map<EActionType, RS_CommandAction*> Cards;
};


struct RSData_Map
{
private:

    RSData_Player playerData[2];
    std::vector<RSData_Piece*> board;
    // pieces
    std::vector<RSData_Piece> pieces;

    EGameState gameState;
    static constexpr inline int CoordToID(uint8_t row, uint8_t col)  {
        return row * MAP_SIZE + col;
    }
public:

    RSData_Map();
    const EGameState GetGameState() const;
    void SetGameState(EGameState newState);
    RSData_Piece* getPiece(uint8_t row, uint8_t col);
    RSData_Player* getPlayer(bool isPlayer1);
};