#include <iostream>
#include <set>
#include <map>

#include "Connections.h"


int printRoute(Connections&, SystemInfo&, std::string, std::string, std::set<std::string>, std::set<std::string>);
void printForm();
void printHead();
std::string getHTMLColour(SystemInfo& info, int system);
