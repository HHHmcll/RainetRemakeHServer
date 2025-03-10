#pragma once
#include "Enums.h"
#include <string>
#include <vector>
#include <memory>
struct RSData_Map;
struct RSData_Command;
struct RSData_Player;
struct RSData_Slot;

enum class EBlock_Status {
	Pass,	// Action Accepted by a Block Check
	Block,	// Action Rejected by a Block Check
	Ignored	// Action is not Related To this Block Check
};

class RS_CommandAction {
public:
	// Called directly before Do. Will not call Do if returned false;
	// Note that this will be called from CreateAction()
	// NOT THE INSTANCE STORED IN RSData_Map
	virtual bool CanDo(const RSData_Command& command, const RSData_Map& map) const = 0;
	// Called if CanDo is true
	// Note that this will be called from CreateAction()
	// NOT THE INSTANCE STORED IN RSData_Map
	// Return true if this move need to be logged
	virtual bool Do(RSData_Command& command, RSData_Map& map) const = 0;


};

class RS_TerminalCard : public RS_CommandAction{
public:
	// Create a object of self
	virtual std::shared_ptr<RS_TerminalCard> CreateNewObject(void* meta) const = 0;
	//Return true if this slot is eligible for this CommandAction
	virtual bool Is(const RSData_Slot* slot) const = 0;
};

typedef const RS_CommandAction* (*CreateActionFunction)(void) ;

class RS_CommandActionManager{
	
	static CreateActionFunction AllStaticActions[EActionType::Num];
	friend class RS_CommandActionCreateFunction;
public:
	template <typename ActionClass>
	static const ActionClass* GetStaticAction();
	static const RS_CommandAction* GetStaticAction(EActionType actionType);
};



class RS_CommandActionCreateFunction {
public:
	RS_CommandActionCreateFunction(EActionType actionType, CreateActionFunction getStaticFunction);
};

template<typename ActionClass>
const ActionClass* RS_CommandActionManager::GetStaticAction()
{
	static_assert(has_static_member<ActionClass>::value);
	return dynamic_cast<const ActionClass*>(GetStaticAction(ActionClass::StaticType));
}
