#include "Misc.h"

namespace Misc
{
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::set<std::string> getAvoidSet(std::map<std::string, std::string> g)
{
	std::set<std::string> out;
	if (g["frigate"] == "Frigate") out.insert("Frigate");
	if (g["reduced"] == "Reduced") out.insert("Reduced");
	if (g["crit"] == "Crit") out.insert("Crit");
	if (g["eol"] == "EOL") out.insert("EOL");
	if (g["gate"] == "Gate") out.insert("EOL");
	if (g["wh"] == "WH") out.insert("WH");
	if (g["jb"] == "JB") out.insert("JB");
	if (g["hs"] == "HS") out.insert("HS");
	if (g["ls"] == "LS") out.insert("LS");
	if (g["ns"] == "NS") out.insert("NS");
	if (g["js"] == "JS") out.insert("JS");
	if (g["thera"] == "Thera") out.insert("Thera");
	return out;
}

std::string lowerCase(std::string in)
{
	std::string out = in;
	std:transform(out.begin(), out.end(), out.begin(), ::tolower);
	return out;
}


}
