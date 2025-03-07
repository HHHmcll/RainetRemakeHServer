#include "GetOutput.h"
#include "RS_IOManager.h"
RS_CommandAction* GetStaticGetOutput()
{
	static CA_GetOutput instance = CA_GetOutput();
	return &instance;
}
std::shared_ptr<RS_CommandAction> CA_GetOutput::CreateNewObject(void* meta)
{
	return std::shared_ptr<RS_CommandAction>(nullptr);
}

bool CA_GetOutput::CanDo(RSData_Command& command, RSData_Map& map)
{
	return true;
}

bool CA_GetOutput::Do(RSData_Command& command, RSData_Map& map)
{
	RS_IOManager::Unhold();
	return true;
}

bool CA_GetOutput::Block(RSData_Player* owner, RSData_Command& command, RSData_Map& map)
{
	return false;
}
