#include "LineBoost.h"
#include "Enums.h"

void CA_LineBoost::Initialize(EPlayerType owner, void* meta)
{
}

bool CA_LineBoost::CanDo(RSData_Command& command, RSData_Map& map)
{
	return false;
}

bool CA_LineBoost::Do(RSData_Command& command, RSData_Map& map, std::vector<uint8_t>& outputBuffer)
{
	return false;
}

bool CA_LineBoost::Block(RSData_Command& command, RSData_Map& map)
{
	return false;
}

RS_CommandAction* CreateLineBoost() {
	return new CA_LineBoost();
}

RS_CommandActionCreateFunction createLineBoostFunction = RS_CommandActionCreateFunction(EActionType::LineBoost, &CreateLineBoost);