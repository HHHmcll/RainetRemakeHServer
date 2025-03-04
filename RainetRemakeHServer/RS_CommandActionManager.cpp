#include "RS_CommandActionManager.h"

RS_CommandAction* RS_CommandActionManager::CreateAction(EActionType actionType)
{
	auto createFunction = AllActions[unsigned long long(actionType)];
	if (createFunction){
		return createFunction();
	}
	return nullptr;
}

RS_CommandActionCreateFunction::RS_CommandActionCreateFunction(EActionType actionType, CreateActionFunction creationFunction) {
	RS_CommandActionManager::AllActions[unsigned long long(actionType)] = creationFunction;
}
