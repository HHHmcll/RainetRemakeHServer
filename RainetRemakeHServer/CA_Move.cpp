#include "CA_Move.h"
#include "Enums.h"
#include "RSData_Map.h"
#include "RSData_Command.h"

CA_Move::CA_Move(){}

bool CA_Move::CanDo(const RSData_Command& command, const RSData_Map& map) const
{


    return true;
}
bool CA_Move::Do(RSData_Command& command, RSData_Map& map) const
{

    return true;
}


const RS_CommandAction* GetStaticMove() {
    static CA_Move MoveStatic = CA_Move();
    return &MoveStatic;
}


RS_CommandActionCreateFunction createMoveFunction = RS_CommandActionCreateFunction(EActionType::Move, &GetStaticMove);