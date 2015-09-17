#include <vector>
#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include <algorithm>

namespace Misc
{
	std::vector<std::string> split(const std::string &s, char delim);
	std::set<std::string> getAvoidSet(std::map<std::string, std::string> g);
	std::string lowerCase(std::string);
}
