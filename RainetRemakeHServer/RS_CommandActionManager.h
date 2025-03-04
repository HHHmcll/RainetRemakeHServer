#pragma once
#include "Enums.h"
#include <vector>
struct RSData_Map;
struct RSData_Command;

class RS_CommandAction {
public:
	// Initialization code, will be called after constructor and before being used.
	// Please put raw data initialization in constructor and avoid logic here until no other choices;
	virtual void Initialize(EPlayerType owner, void* meta) = 0;
	// Called directly before Do. Will not call Do if returned false;
	// Note that this will be called from CreateAction()
	// NOT THE INSTANCE STORED IN RSData_Map
	virtual bool CanDo(RSData_Command& command, RSData_Map& map) = 0;
	// Called if CanDo is true
	// Note that this will be called from CreateAction()
	// NOT THE INSTANCE STORED IN RSData_Map
	virtual bool Do(RSData_Command& command, RSData_Map& map, std::vector<uint8_t>& outputBuffer) = 0;
	// Return if CommandAction stored in command should be blocked based on ***this***
	// Will not process command if return false
	// Will be called from the instance stored in RSData_Map
	virtual bool Block(RSData_Command& command, RSData_Map& map) = 0;
};

typedef RS_CommandAction* (*CreateActionFunction)(void) ;

class RS_CommandActionManager{
	static CreateActionFunction AllActions[unsigned long long (EActionType::Num)];
	friend class RS_CommandActionCreateFunction;
public:
	static RS_CommandAction* CreateAction(EActionType actionType);
};



class RS_CommandActionCreateFunction {
public:
	RS_CommandActionCreateFunction(EActionType actionType, CreateActionFunction creationFunction);
};