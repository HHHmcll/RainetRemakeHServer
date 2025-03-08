#pragma once
#include <stdint.h>
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
	
	InitializeTerminal,
	InitializePieces,
	VisualEffet,
	Error,

	Move,
	
	LineBoost,
	FireWall,
	VirusCheck,
	NotFound,
	Rabbit,
	ZeroDayAttack,
	SandBox,

	Num
};


enum EGameState : uint8_t{
	Initialization,
	WaitingPlayer1,
	WaitingPlayer2,
	End
};
