
#include "RS_IOManager.h"
#include "RSData_Command.h"
#include "RSData_Map.h"
#include "RS_CommandProcesser.h"



class RS_GameLogManager {

};

class RS_GameInstance {
	RSData_Map _mapData;

private:
	RS_GameInstance();
	static RS_GameInstance& Get() {
		static RS_GameInstance instance = RS_GameInstance();
		return instance;
	}
public:
	static void Init(char** Args) {
		// Parse Args
		// Create new Map
		// Get Ready to Tick
	}
	static bool Tick() {
		RSData_Command command;
		RS_IOManager::FetchCommand(command);
		std::vector<uint8_t> outputBuffer = std::vector<uint8_t>();
		RS_CommandProcesser::ProcessCommand(command, Get()._mapData, outputBuffer);
		
		RS_GameLogManager::LogCommand();
		RS_IOManager::WriteResponse(command);

	}
	static void Exit() {
		RS_GameLogManager::Save();
		// Cleanup
	}
};
int main(char** Args) {
	RS_GameInstance::Init(Args);
	while (RS_GameInstance::Tick());
	RS_GameInstance::Exit();
}