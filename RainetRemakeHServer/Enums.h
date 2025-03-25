#pragma once
#include <stdint.h>
#include <utility>
enum EPlayerType : uint8_t {
	Player1,
	Player2,
	Viewer,
	God,
	Everyone,
	Empty
};

enum EPieceType : uint8_t
{
	Link,
	Virus,
	Unknown,
	Null
};

enum EActionType : uint8_t {
	GetOutput,
	BoardDisplay,
	
	InitializeBoard,
	VisualEffet,
	End,
	Error,

	Move,
	
	LineBoost,
	FireWall,
	VirusCheck,
	NotFound,
	RabbitTrap,
	ZeroDayAttack,
	SandBox,

	Num
};


enum EGameState : uint8_t{
	Initialization,
	WaitingPlayer1,
	WaitingPlayer2,
	EndGame
};

template <typename T>
class has_static_member {
private:
	template <typename U>
	static auto test(int) -> decltype(U::StaticType, std::true_type());

	template <typename>
	static std::false_type test(...);

public:
	static constexpr bool value = decltype(test<T>(0))::value;
};
