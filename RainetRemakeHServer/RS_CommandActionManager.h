#pragma once
#include "Enums.h"
#include <vector>
struct RSData_Map;
struct RSData_Command;

class RS_CommandAction {
public:
	virtual void Initialize(EPlayerType owner, void* meta) = 0;
	virtual bool CanDo(RSData_Command& command, RSData_Map& map) = 0;
	virtual bool Do(RSData_Command& command, RSData_Map& map, std::vector<uint8_t>& outputBuffer) = 0;
	virtual bool Usable() = 0;
};

typedef RS_CommandAction* (*CreateActionFunction)(void) ;

class RS_CommandActionManager{
	static CreateActionFunction AllActions[unsigned long long (EActionType::Num)];
	friend class RS_CommandActionCreateFunction;
public:
	RS_CommandAction* CreateAction(EActionType actionType);
};



class RS_CommandActionCreateFunction {
public:
	RS_CommandActionCreateFunction(EActionType actionType, CreateActionFunction creationFunction);
};