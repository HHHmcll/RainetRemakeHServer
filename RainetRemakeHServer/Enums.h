#pragma once

enum class EPlayerType : uint8_t {
	Player1,
	Player2,
	Viewer,
	God,
	Empty
};

enum class EPawnType : uint8_t
{
	Link,
	Virus,
	Unknown,
	Null
};

enum class EActionType : uint8_t {
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
