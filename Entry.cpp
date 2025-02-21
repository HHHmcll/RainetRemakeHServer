class RSData_Command {

};

class RS_CommandParser{

};

class RSData_Map {

};

class RS_CommandProcesser {

};



class RS_IOManager {

};

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
		string rawCommand = RS_IOManager::ReadCommand();
		RSData_Command command = RS_CommandParser::ParseCommand(rawCommand);
		RS_CommandProcesser::ProcessCommand(command, Get()._mapData);
		
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