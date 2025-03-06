
#include "RSData_Command.h"
#include "RSData_Map.h"

#include "RS_IOManager.h"
#include "RS_CommandProcesser.h"
#include "RS_Logger.h"

class RS_GameInstance {
	RSData_Map* mapData;
	RS_Logger* logger;
private:
	RS_GameInstance():mapData(nullptr),logger(nullptr){}
	~RS_GameInstance(){
		delete mapData;
		delete logger;
	}
	static RS_GameInstance& Get() {
		static RS_GameInstance instance = RS_GameInstance();
		return instance;
	}
public:
	static void Init(char** Args) {
		Get().mapData = new RSData_Map(4);
		Get().logger = new RS_Logger("./out.txt");
		// Parse Args
		// Create new Map
		// Get Ready to Tick
		RS_IOManager::Init();
	}
	static bool Tick() {
		RSData_Command command;
		if (!RS_IOManager::HasCommand()) {
			if (RS_IOManager::ShouldResolve()) {
				RS_IOManager::ResolveOutput();
				return true;
			}
		}

		RS_IOManager::FetchCommand(command);

		if (RS_CommandProcesser::ProcessCommand(command, *Get().mapData)) {

		}
		RS_IOManager::ResolveOutput();

		return true;
	}
	static void Exit() {

		//RS_GameLogManager::Save();
		// Cleanup
		
	}
};
int main(char** Args) {
	RS_GameInstance::Init(Args);
	while (RS_GameInstance::Tick());
	RS_GameInstance::Exit();
}