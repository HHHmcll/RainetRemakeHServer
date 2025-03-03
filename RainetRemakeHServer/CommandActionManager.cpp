#include "RS_CommandActionManager.h"

RS_CommandAction* RS_CommandActionManager::CreateAction(EActionType actionType)
{
	return (AllActions[unsigned long long(actionType)])();
}

RS_CommandActionCreateFunction::RS_CommandActionCreateFunction(EActionType actionType, CreateActionFunction creationFunction) {
	RS_CommandActionManager::AllActions[unsigned long long(actionType)] = creationFunction;
}
