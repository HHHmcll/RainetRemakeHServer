#include "CA_VisualEffet.h"
#include "RS_IOManager.h"
#include "RSData_Command.h"
#include "RS_IOManager.h"

const RS_CommandAction* GetStaticVisualEffet()
{
	static CA_VisualEffet instance = CA_VisualEffet();
	return &instance;
}

bool CA_VisualEffet::CanDo(const RSData_Command& command, const RSData_Map& map) const
{
	if(command.Data.TerminalSetup <= 0 ){
		return false;
	}
	return true;
}

bool CA_VisualEffet::Do(RSData_Command& command, RSData_Map& map) const
{
	uint8_t* buffer = new uint8_t[command.Data.TerminalSetup + 6];
	buffer[0] = EPlayerType::Everyone;
	buffer[1] = EActionType::VisualEffet;
	*(reinterpret_cast<uint32_t*>(buffer + 2)) = command.Data.TerminalSetup;
	memcpy(buffer + 6,command.Meta.get(),command.Data.TerminalSetup);
	RS_IOManager::QueueOutput(buffer,command.Data.TerminalSetup);
	return false;
}


RS_CommandActionCreateFunction createVisualEffetFunction = RS_CommandActionCreateFunction(EActionType::VisualEffet, &GetStaticVisualEffet);