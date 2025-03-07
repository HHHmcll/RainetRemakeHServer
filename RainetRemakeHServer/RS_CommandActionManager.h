#pragma once
#include "Enums.h"
#include <string>
#include <vector>
#include <memory>
struct RSData_Map;
struct RSData_Command;
struct RSData_Player;

enum class EBlock_Status {
	Pass,	// Action Accepted by a Block Check
	Block,	// Action Rejected by a Block Check
	Ignored	// Action is not Related To this Block Check
};

class RS_CommandAction {
public:
	// Create a object of self
	virtual std::shared_ptr<RS_CommandAction> CreateNewObject(void* meta) = 0;
	// Called directly before Do. Will not call Do if returned false;
	// Note that this will be called from CreateAction()
	// NOT THE INSTANCE STORED IN RSData_Map
	virtual bool CanDo(RSData_Command& command, RSData_Map& map) = 0;
	// Called if CanDo is true
	// Note that this will be called from CreateAction()
	// NOT THE INSTANCE STORED IN RSData_Map
	// Return true if this move need to be logged
	virtual bool Do(RSData_Command& command, RSData_Map& map) = 0;
	// Return if CommandAction stored in command should be blocked based on ***this***
	// Will not process command if return false
	// Will be called from the instance stored in RSData_Map
	virtual EBlock_Status Block(RSData_Player* owner, RSData_Command& command, RSData_Map& map) = 0;

	
};

typedef RS_CommandAction* (*CreateActionFunction)(void) ;

class RS_CommandActionManager{
	
	static CreateActionFunction AllStaticActions[EActionType::Num];
	friend class RS_CommandActionCreateFunction;
public:
	static RS_CommandAction* GetStaticAction(EActionType actionType);
};



class RS_CommandActionCreateFunction {
public:
	RS_CommandActionCreateFunction(EActionType actionType, CreateActionFunction getStaticFunction);
};