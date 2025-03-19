#include "CA_End.h"
#include "RS_IOManager.h"
const RS_CommandAction* GetStaticEnd()
{
	static CA_End instance = CA_End();
	return &instance;
}

bool CA_End::CanDo(const RSData_Command& command, const RSData_Map& map) const
{
	return false;
}

bool CA_End::Do(RSData_Command& command, RSData_Map& map) const
{
	return false;
}


RS_CommandActionCreateFunction createEndFunction = RS_CommandActionCreateFunction(EActionType::End, &GetStaticEnd);