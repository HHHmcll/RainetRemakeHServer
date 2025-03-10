
#include "RS_GameInstance.h"

#include "RSData_Command.h"
#include "RSData_Map.h"

#include "RS_IOManager.h"
#include "RS_CommandProcesser.h"
#include "RS_Logger.h"


RS_GameInstance::RS_GameInstance() :mapData(nullptr), logger(nullptr),ShouldAbort(false) {}

RS_GameInstance::~RS_GameInstance() {
	delete mapData;
	delete logger;
}

RS_GameInstance& RS_GameInstance::Get() {
	static RS_GameInstance instance = RS_GameInstance();
	return instance;
}

void RS_GameInstance::Init(int argc, char* argv[]) {

	Get().mapData = new RSData_Map(4);
	Get().logger = new RS_Logger("./out.txt");
	// Parse Args
	// Create new Map
	// Get Ready to Tick
	RS_IOManager::Init();
}

bool RS_GameInstance::Tick() {
	RSData_Command command;
	if (!RS_IOManager::HasCommand()) {
		if (RS_IOManager::ShouldResolve()) {
			RS_IOManager::ResolveOutput();
			return !Get().ShouldAbort;
		}
	}

	RS_IOManager::FetchCommand(command);

	if (RS_CommandProcesser::ProcessCommand(command, *Get().mapData)) {
		Get().logger->LogMove(command);
	}
	RS_IOManager::ResolveOutput();

	return !Get().ShouldAbort;
}

void RS_GameInstance::Exit() {

	Get().logger->Save();

}

void RS_GameInstance::Abort()
{
	Get().ShouldAbort = true;
}
