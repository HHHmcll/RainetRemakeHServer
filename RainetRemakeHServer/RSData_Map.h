#pragma once

#include "Enums.h"
#include <map>
#include <vector>
#include <memory>

#define MAP_SIZE (8)
struct RSData_Piece
{
    RSData_Player* Player;
    EPieceType Type;
    RSData_Piece(RSData_Player* player, EPieceType type):Player(player), Type(type){}
};

class RS_CommandAction;

struct RSData_Player
{
    uint8_t PlayerID;
    uint8_t LinkAte, LinkEnter;
    uint8_t VirusAte, VirusEnter;
    std::map<EActionType, std::shared_ptr<RS_CommandAction>> Cards;
    RSData_Piece pieces[MAP_SIZE];
    
    RSData_Player(uint8_t playerID):
    PlayerID(playerID),LinkAte(0),LinkEnter(0),VirusAte(0),VirusEnter(0),
    pieces{RSData_Piece(this,EPieceType::Unknown), RSData_Piece(this,EPieceType::Unknown),
        RSData_Piece(this,EPieceType::Unknown), RSData_Piece(this,EPieceType::Unknown),
        RSData_Piece(this,EPieceType::Unknown), RSData_Piece(this,EPieceType::Unknown),
        RSData_Piece(this,EPieceType::Unknown), RSData_Piece(this,EPieceType::Unknown)}
    {}
};


struct RSData_Map
{
private:

    RSData_Player playerData[2];
    std::vector<RSData_Piece*> board;
    // pieces
    EGameState gameState;
    static constexpr inline int CoordToID(uint8_t row, uint8_t col)  {
        return row * MAP_SIZE + col;
    }
public:

    const uint32_t MaxTerminals;
    RSData_Map(uint32_t maxTerminals);
    inline const EGameState GetGameState() const;
    inline void SetGameState(EGameState newState);
    inline RSData_Piece* getPiece(uint8_t row, uint8_t col);
    inline RSData_Player& getPlayer(bool isPlayer1);
};