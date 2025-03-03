#pragma once

enum class EPlayerType : unsigned char {
	Player1,
	Player2,
	Viewer,
	God,
	Empty
};

enum class EPawnType : unsigned char
{
	Link,
	Virus,
	Unknown,
	Null
};

enum class EActionType : unsigned char {
	BoardDisplay = 0,
	AddPlayer,

	InitializeTerminal,
	InitializePieces,

	Move,
	LineBoost,
	FireWall,
	VirusCheck,
	NotFound,
	Rabbit,
	ZeroDayAttack,
	SandBox,

	ArtRequirements,
	ErrorRequirements,
	Num
};
