#pragma once

#include "Enums.h"
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <array>

#define MAP_SIZE  8 
struct RSData_Player;

class RS_TerminalCard;

struct RSData_Slot;

struct RSData_Piece
{
    RSData_Player* Player;
    EPieceType Type;
    RSData_Slot* Slot;
    bool revealed;
    RSData_Piece(RSData_Player* player, EPieceType type);
};

struct RSData_Slot
{
    const bool bOnBoard;
    RSData_Piece* Piece;
    const size_t SlotID;
private:
    template< std::size_t... Indices>
    friend constexpr  auto make_array_helper(std::index_sequence<Indices...>)->std::array<RSData_Slot, sizeof...(Indices)>;
    RSData_Slot(size_t id, bool OnBoard):SlotID(id), bOnBoard(OnBoard), Piece(nullptr){}
};

struct RSData_Player
{
    uint8_t PlayerID;
    uint8_t LinkAte, LinkEnter;
    uint8_t VirusAte, VirusEnter;
    std::map<EActionType, std::shared_ptr<RS_TerminalCard>> Cards;
    RSData_Piece pieces[MAP_SIZE];

    RSData_Player(uint8_t playerID);
    template<typename TerminalClass>
    TerminalClass* GetTerminal();
    template<typename TerminalClass>
    const TerminalClass* GetTerminal() const;
    // will return true and stop following iterations if callback return true
    // return false otherwise
    bool ForEachTerminal(std::function<bool(RS_TerminalCard*)> callback);
};


struct RSData_Map
{
private:

    RSData_Player playerData[2];
    std::array<RSData_Slot, MAP_SIZE* MAP_SIZE> board;

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
    RSData_Slot* getPieceSlot(uint8_t row, uint8_t col);
    RSData_Piece* getPiece(uint8_t row, uint8_t col);
    const RSData_Slot* getPieceSlot(uint8_t row, uint8_t col)const;
    const RSData_Piece* getPiece(uint8_t row, uint8_t col)const;
    RSData_Player& getPlayer(bool isPlayer1);
    const RSData_Player& getPlayer(bool isPlayer1) const;

    bool IsTerminal(EPlayerType player, EActionType terminal,const RSData_Slot* slot) const;
    // will return true and stop following iterations if callback return true
    // return false otherwise
    bool ForEachPlayer(std::function<bool(RSData_Player*)> callback);
    bool GetCoordFromSlot(RSData_Slot* slot, uint8_t& row, uint8_t& col);
    bool CheckPlayerType(EPlayerType playerType) const;
};

template<typename TerminalClass>
TerminalClass* RSData_Player::GetTerminal() {
    static_assert(has_static_member<TerminalClass>::value);
    if (!Cards.contains(TerminalClass::StaticType)) {
        return nullptr;
    }
    return dynamic_cast<TerminalClass*>(Cards.at(TerminalClass::StaticType).get());
}

template<typename TerminalClass>
const TerminalClass* RSData_Player::GetTerminal() const {
    static_assert(has_static_member<TerminalClass>::value);
    if (!Cards.contains(TerminalClass::StaticType)) {
        return nullptr;
    }
    return dynamic_cast<TerminalClass*>(Cards.at(TerminalClass::StaticType).get());
}