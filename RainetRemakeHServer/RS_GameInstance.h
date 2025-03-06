
struct RSData_Map;
class RS_Logger;

class RS_GameInstance {
	RSData_Map* mapData;
	RS_Logger* logger;
	bool ShouldAbort;
private:
	RS_GameInstance();
	~RS_GameInstance();
	static RS_GameInstance& Get();
public:
	static void Init(int argc, char* argv[]);
	static bool Tick();
	static void Exit();

	static void Abort();
};