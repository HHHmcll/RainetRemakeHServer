#pragma once
#include "RS_CommandActionManager.h"

struct RSData_Piece;

class CA_RabbitTrap :public RS_TerminalCard {
private:
	CA_RabbitTrap();
	CA_RabbitTrap(RSData_Player* owner);
	const RSData_Player* Owner;
	friend const RS_CommandAction* GetStaticRabbitTrap();
	bool used;
	std::vector<RSData_Slot*> fakeSlots;
public:

	RSData_Slot* TrappedSlot;
	static const EActionType StaticType = EActionType::RabbitTrap;
	std::shared_ptr<RS_TerminalCard> CreateNewObject(RSData_Player* owner) const override;
	bool CanDo(const RSData_Command& command, const RSData_Map& map) const override;
	bool Do(RSData_Command& command, RSData_Map& map) const override;
	bool Is(const RSData_Slot* slot) const override;
	void WriteToBuffer(const bool ShouldHide, std::vector<uint8_t>& buffer, const RSData_Map& map) const;
};

