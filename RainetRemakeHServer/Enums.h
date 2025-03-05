#pragma once

enum EPlayerType : uint8_t {
	Player1,
	Player2,
	Viewer,
	God,
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
	Start,
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
