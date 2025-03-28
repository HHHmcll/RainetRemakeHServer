#include "RS_CommandActionManager.h"


CreateActionFunction RS_CommandActionManager::AllStaticActions[EActionType::Num] = {};

const RS_CommandAction* RS_CommandActionManager::GetStaticAction(EActionType actionType)
{
	if(actionType >= EActionType::Num){
		return nullptr;
	}
	auto createFunction = AllStaticActions[unsigned long long(actionType)];
	if (createFunction){
		return createFunction();
	}
	return nullptr;
}


RS_CommandActionCreateFunction::RS_CommandActionCreateFunction(EActionType actionType, CreateActionFunction getStaticFunction) {
	RS_CommandActionManager::AllStaticActions[actionType] = getStaticFunction;
}
