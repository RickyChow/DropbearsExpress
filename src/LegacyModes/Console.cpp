#include "Console.h"


void runInterface(std::string path)
{
	std::cout << "Dropbears wormhole route planner \n";
	Siggy s = Siggy();
	std::cout << "Loading k-space connections\n";
	SystemInfo info = SystemInfo(path.c_str());
	Connections c = Connections(path.c_str(), &info);

	std::set<std::string> ignore_sVect;

	std::cout << "Loading siggy wormhole connections\n\r";
	c.loadSiggyConnections(s);
	std::cout << "Loading thera connections\n\r";
	c.loadTheraConnections(s);
	info.updateWithSiggy(s);

	std::set<std::string> avoidSet;

	while(true)
	{
		avoidSet.clear();
		std::string start, finish, line;

		std::cout << "\n\r\n\rDropbears Express by InternetHero\n\rStart, Finish, Avoid tags \n\rSystem tags:HS,LS,NS,JS    Connection tags: Gate,WH,EOL,Frigate,Thera \n\r>>";

		std::getline(std::cin, line);
		std::vector<std::string> inputs = split<std::string>(line);

		if (inputs.size() == 1 && inputs[0] == ".update")
		{
			std::cout << "Updating map...\n\r";
			updateAll(s, c, info, path);
			std::cout << "Map updated! \n\r";
			continue;
		}else if (inputs.size() < 2)
		{
			std::cout << "insufficient inputs \n\r";
			continue;
		}


		start = inputs[0];
		finish = inputs[1];

		for (std::vector<std::string>::iterator i = inputs.begin(); i != inputs.end(); i++)
		{
			if (*i != start && *i != finish)
				avoidSet.insert(*i);
		}

		std::cout << "Routing..." << std::endl << std::endl;
		printRoute(c, info, start, finish, avoidSet, ignore_sVect);
		std::cout << std::endl << std::endl;
	}
}

void updateAll(Siggy& s, Connections& c, SystemInfo& info, std::string dbPath)
{
	s.update();
	c.clearAllConnections();
	c.loadConnections(dbPath.c_str());
	c.loadSiggyConnections(s);
	c.loadTheraConnections(s);
	info.updateWithSiggy(s);
}




template<typename T>
std::vector<T> split(const std::string& line) {
	std::istringstream is(line);
	return std::vector<T>(std::istream_iterator<T>(is), std::istream_iterator<T>());
}
