#include "LineBoost.h"
#include "Enums.h"

CA_LineBoost::CA_LineBoost() {

}

std::shared_ptr<RS_CommandAction> CA_LineBoost::CreateNewObject(void* meta)
{
	return std::shared_ptr<RS_CommandAction>(new CA_LineBoost());
}

bool CA_LineBoost::CanDo(RSData_Command& command, RSData_Map& map)
{
	return false;
}

bool CA_LineBoost::Do(RSData_Command& command, RSData_Map& map)
{
	return false;
}

bool CA_LineBoost::Block(RSData_Command& command, RSData_Map& map)
{
	return false;
}


RS_CommandAction* GetStaticLineBoost() {
	static CA_LineBoost LineBoostStatic = CA_LineBoost();
	return &LineBoostStatic;
}

RS_CommandActionCreateFunction createLineBoostFunction = RS_CommandActionCreateFunction(EActionType::LineBoost, &GetStaticLineBoost);