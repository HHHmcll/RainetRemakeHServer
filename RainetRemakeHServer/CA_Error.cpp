#include "CA_Error.h"
#include "RS_IOManager.h"
const RS_CommandAction* GetStaticError()
{
	static CA_Error instance = CA_Error();
	return &instance;
}

bool CA_Error::CanDo(const RSData_Command& command, const RSData_Map& map) const
{
	return false;
}

bool CA_Error::Do(RSData_Command& command, RSData_Map& map) const
{
	return false;
}


RS_CommandActionCreateFunction createErrorFunction = RS_CommandActionCreateFunction(EActionType::Error, &GetStaticError);