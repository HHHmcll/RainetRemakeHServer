#include <vector>
struct RSData_Command;
struct RSData_Map;

class RS_CommandProcesser {
public:
    static bool ProcessCommand(RSData_Command& command, RSData_Map& map, std::vector<uint8_t>& output);
};