#pragma once
#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <set>

#include "utils/Misc.h"
#include "utils/Siggy.h"
#include "utils/SystemTypeGetter.h"

struct System{
	int sID;
	std::string sName;
	float security;
	std::string type; //0=HS, 1=LS, 2=NS, 3=WH
	std::string nickname;
};

class SystemInfo : public SystemTypeGetter
{
	std::map<std::string, int>sID;
	std::map<int, System>systems;
	std::set<int> HSsystems,LSsystems, NSsystems, WHsystems, Thera;

public:
	SystemInfo(){}
	SystemInfo(const char* filePath);
	int getSID(std::string);
	std::string getSName(int);
	float getSecurity(int);
	std::string getType(int id);
	std::string getNickname(int id);
	void updateWithSiggy(Siggy);
	std::set<int> getHS();
	std::set<int> getLS();
	std::set<int> getNS();
	std::set<int> getWH();
	std::set<int> getThera();

};

#endif
