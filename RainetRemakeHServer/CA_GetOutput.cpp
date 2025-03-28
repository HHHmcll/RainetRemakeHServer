#include "CA_GetOutput.h"
#include "RS_IOManager.h"
const RS_CommandAction* GetStaticGetOutput()
{
	static CA_GetOutput instance = CA_GetOutput();
	return &instance;
}

bool CA_GetOutput::CanDo(const RSData_Command& command, const RSData_Map& map) const
{
	return true;
}

bool CA_GetOutput::Do(RSData_Command& command, RSData_Map& map) const
{
	RS_IOManager::Unhold();
	return false;
}


RS_CommandActionCreateFunction createGetOutputFunction = RS_CommandActionCreateFunction(EActionType::GetOutput, &GetStaticGetOutput);