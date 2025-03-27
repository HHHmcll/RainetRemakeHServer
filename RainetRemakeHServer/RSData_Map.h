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
struct RSData_Command;

class RS_Delegate {
    public:
        RS_Delegate() = default;
        using Lambda = std::function<void(RSData_Command&)>;
    
        void Add(EActionType key, Lambda func) {
            mapping[key] = func;
        }
        void BroadCast(RSData_Command& arg){
            for(auto& func : mapping){
                func.second(arg);
            }
        }
        void Execute(EActionType key, RSData_Command& arg) {
            auto func = mapping[key];
            if (func) {
                func(arg);
            }
        }
        void Remove(EActionType key) {
            mapping.erase(key);
        }
    private:
        std::map<EActionType, Lambda> mapping;
};

struct RSData_Piece
{
    RSData_Player* Player;
    EPieceType Type;
    RSData_Slot* Slot;

    bool revealed;
    RS_Delegate OnPieceRemovedFromBoard;
    RSData_Piece(RSData_Player* player, EPieceType type);
};

struct RSData_Slot
{
    const bool bOnBoard;
    RSData_Piece* Piece;
    const size_t SlotID;
private:
    friend struct RSData_Map;
    template< std::size_t... Indices>
    friend constexpr  auto make_array_helper(std::index_sequence<Indices...>, bool isOnBoard)->std::array<RSData_Slot, sizeof...(Indices)>;
    RSData_Slot(size_t id, bool OnBoard):SlotID(id), bOnBoard(OnBoard), Piece(nullptr){}
};

struct RSData_Player
{
    uint8_t PlayerID;
    uint8_t AteCount[2], EnterCount[2];
    
    std::map<EActionType, std::shared_ptr<RS_TerminalCard>> Cards;
    RSData_Piece pieces[MAP_SIZE];
    std::vector<RSData_Piece*> ServerSlots;
    std::vector<RSData_Piece*> CaptureSlot[2];
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
    RSData_Slot ExitToServerSlot[2];
    // pieces
    EGameState gameState;
    static constexpr inline int CoordToID(uint8_t row, uint8_t col)  {
        return row * MAP_SIZE + col;
    }

public:
    RS_Delegate MapPreMoveDelegate;
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

    EPlayerType IsTerminal(EActionType terminal,const RSData_Slot* slot) const;
    // will return true and stop following iterations if callback return true
    // return false otherwise
    bool ForEachPlayer(std::function<bool(RSData_Player*)> callback);
    // coord : high : A offboard, low : playerID
    // high: row, low: col
    uint8_t GetCoordFromSlot(RSData_Slot* slot) const;

    bool CheckPlayerType(EPlayerType playerType) const;
    // Game End if true;
    bool EndRoundCheck();

    void PerformMove(RSData_Player& playerRef, RSData_Command& command, RSData_Slot* commandSlotFrom, RSData_Slot* commandSlotTo);
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