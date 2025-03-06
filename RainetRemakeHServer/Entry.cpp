#include "RS_GameInstance.h"

int main(int argc, char *argv[]) {
	RS_GameInstance::Init(argc, argv);
	while (RS_GameInstance::Tick());
	RS_GameInstance::Exit();
}