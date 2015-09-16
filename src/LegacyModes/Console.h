#include "Connections.h"
#include "Printing.h"

void updateAll(Siggy&, Connections&, SystemInfo&, std::string dbPath);
void runInterface(int, char*);
template<typename T>
std::vector<T> split(const std::string& line);
