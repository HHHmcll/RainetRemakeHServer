#pragma once

#include "Enums.h"
#include <map>
#include <vector>
#include <memory>
#include <functional>

#define MAP_SIZE  8 
struct RSData_Player;
struct RSData_Command;

class RS_CommandAction;

struct RSData_Piece
{
    RSData_Player* Player;
    EPieceType Type;
    RSData_Piece** Slot;
    RSData_Piece(RSData_Player* player, EPieceType type);
};


struct RSData_Player
{
    uint8_t PlayerID;
    uint8_t LinkAte, LinkEnter;
    uint8_t VirusAte, VirusEnter;
    std::map<EActionType, std::shared_ptr<RS_CommandAction>> Cards;
    RSData_Piece pieces[MAP_SIZE];
    
    RSData_Player(uint8_t playerID);
    bool ForEachTerminal(std::function<bool(RS_CommandAction*)> callback);
};


struct RSData_Map
{
private:

    RSData_Player playerData[2];
    RSData_Piece* board[MAP_SIZE * MAP_SIZE];
    // pieces
    EGameState gameState;
    static constexpr inline int CoordToID(uint8_t row, uint8_t col)  {
        return row * MAP_SIZE + col;
    }

public:

    const uint32_t MaxTerminals;
    RSData_Map(uint32_t maxTerminals);
    const EGameState GetGameState() const;
    void SetGameState(EGameState newState);
    RSData_Piece** getPieceSlot(uint8_t row, uint8_t col);
    RSData_Piece* getPiece(uint8_t row, uint8_t col);
    RSData_Player& getPlayer(bool isPlayer1);


    // will return true and stop following iteration if callback return true
    // return false otherwise
    bool ForEachPlayer(std::function<bool(RSData_Player*)> callback);
    bool GetCoordFromSlot(RSData_Piece** slot, uint8_t& row, uint8_t& col);
};