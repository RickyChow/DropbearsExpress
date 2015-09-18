#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <fstream>
#include <string>


#include "SystemInfo.h"
#include "Siggy.h"

#define AVOID_WEIGHT 100


class Connections{
	std::map<int, std::map<int,std::set<std::string> > > connections;
	int calculateWeight(std::pair<int, std::set<std::string> >, std::set<std::string>);
	SystemInfo* info;
public:
	Connections(const char*, SystemInfo*);
	void clearAllConnections();
	std::vector<int> getRouteDijkstra(int, int, std::set<std::string> = std::set<std::string>(), std::set<int> = std::set<int>());
	void addConnections(int, int, std::set<std::string>);
	void loadConnections(const char*);
	void loadSiggyConnections(Siggy sData);
	void loadTheraConnections(Siggy sData);
	std::set<std::string> getConnectionType(int from, int to);
};

#endif
