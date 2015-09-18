#include "Connections.h"


Connections::Connections(const char* fileName, SystemInfo* sInfo)
{
	info = sInfo;
	loadConnections(fileName);
}

void Connections::loadConnections(const char* fileName)
{
	sqlite3_stmt *jumpStatement;
	sqlite3 *db;
	sqlite3_open(fileName, &db);
	sqlite3_prepare_v2(db, "SELECT * from mapSolarSystemJumps", -1, &jumpStatement, 0);

	while(sqlite3_step(jumpStatement) == SQLITE_ROW)
	{
		int fromSysID = sqlite3_column_int(jumpStatement, 2);
		int toSysID = sqlite3_column_int(jumpStatement, 3);

		//Try to insert the start connection, if it doesn't exist, then create an empty set
		std::set<std::string> tags;
		tags.insert("Gate");
		addConnections(fromSysID, toSysID, tags);
	}
}

void Connections::clearAllConnections()
{
	connections.clear();
}

void Connections::addConnections(int fromSysID, int toSysID, std::set<std::string> type)
{
	std::set<std::string> toType, fromType;

	toType.insert(info->getType(toSysID));
	toType.insert(type.begin(), type.end());
	fromType.insert(info->getType(fromSysID));
	fromType.insert(type.begin(), type.end());

	connections.insert(make_pair(fromSysID, std::map<int, std::set<std::string> >()));
	connections.insert(make_pair(toSysID, std::map<int, std::set<std::string> >()));

	connections.find(fromSysID)->second.insert(make_pair(toSysID, toType));
	connections.find(toSysID)->second.insert(make_pair(fromSysID, fromType));

}

void Connections::loadSiggyConnections(Siggy sData)
{
	json_spirit::mValue val = sData.getNovaData();
	json_spirit::mObject chainMap_obj = val.get_obj().find("chainMap")->second.get_obj();

	if (chainMap_obj.find("wormholes")->second.type() == json_spirit::obj_type)
	{
		json_spirit::mObject wormHoles_Obj = chainMap_obj.find("wormholes")->second.get_obj();
		for (json_spirit::mObject::iterator i = wormHoles_Obj.begin(); i != wormHoles_Obj.end(); i++)
		{
			 std::set<std::string> tags;
			tags.insert("WH");
			json_spirit::mObject connectionObj = i->second.get_obj();
			std::string from_ws = connectionObj.find("from_system_id")->second.get_str();
			std::string to_ws = connectionObj.find("to_system_id")->second.get_str();

			if (connectionObj.find("frigate_sized")->second.get_str() == "1")
				tags.insert("Frigate");
			if (connectionObj.find("eol")->second.get_str() == "1")
				tags.insert("EOL");
			if (connectionObj.find("mass")->second.get_str() == "1")
				tags.insert("Reduced");
			else if (connectionObj.find("mass")->second.get_str() == "2")
				tags.insert("Crit");


			addConnections(atoi((char*)from_ws.c_str()), atoi((char*)to_ws.c_str()), tags);
		}
	}
	if (chainMap_obj.find("jumpbridges")->second.type() == json_spirit::obj_type)
	{
		json_spirit::mObject jumpBridge_Obj = chainMap_obj.find("jumpbridges")->second.get_obj();
		for (json_spirit::mObject::iterator i = jumpBridge_Obj.begin(); i != jumpBridge_Obj.end(); i++)
		{
			json_spirit::mObject connectionObj = i->second.get_obj();
			std::string from_ws = connectionObj.find("from_system_id")->second.get_str();
			std::string to_ws = connectionObj.find("to_system_id")->second.get_str();

			 std::set<std::string> tags;
			tags.insert("JB");

			addConnections(atoi((char*)from_ws.c_str()), atoi((char*)to_ws.c_str()), tags);
		}
	}
}

void Connections::loadTheraConnections(Siggy sData)
{
	json_spirit::mValue val = sData.getTheraData();
	json_spirit::mObject wormHoles_obj = val.get_obj();
	for (json_spirit::mObject::iterator i = wormHoles_obj.begin(); i != wormHoles_obj.end(); i++)
	{
		//std::cout << i->first << std::endl;
		int to_ws = i->second.get_obj().find("system")->second.get_obj().find("id")->second.get_int();

		//Add connection to thera
		 std::set<std::string> tags;
		tags.insert("Thera");
		tags.insert("JS");
		tags.insert("WH");
		addConnections(31000005, to_ws, tags);
	}
}

std::vector<int> Connections::getRouteDijkstra(int start, int finish, 
        std::set<std::string> avoid,  std::set<int> ignore)
{
    typedef std::pair<int,std::pair<int,int> > QElement;

	std::vector<int> path;

    //Reverse priority queue for pair<distance, <systemID,SystemFrom>>
	std::priority_queue<QElement,std::vector<QElement>, std::greater<QElement> > q;

    //Contains <system visited, where we came from to get to system visited>
	std::map<int, int> visited; 

	q.push(std::make_pair(0, std::make_pair(start,start)));
	
	if (start==finish)
	{
		path.push_back(start);
		return path;
	}
	while(!q.empty())
	{
		//Keep popping stuff from the queue if we've been to the next point already
        //, Otherwise add it to the visited set
		while (!visited.insert(std::make_pair(q.top().second.first, q.top().second.second)).second)
		{
			q.pop();
			if (q.empty())
				return std::vector<int>(0);
		}
		if (q.top().second.first == finish)
		{
			visited.insert(std::make_pair(finish, q.top().second.second));
			path.push_back(finish);
			do
			{
				path.push_back(visited.find(path.back())->second);
			} while (path.back() != start);

			std::reverse(path.begin(), path.end());
			goto finish;
		}
		//Set of destinations from the current point
		std::map<int,  std::set<std::string> > destinations_set = connections.find(q.top().second.first)->second;

		//Iterate through all connected systems
		for (std::map<int, std::set<std::string> >::iterator i = destinations_set.begin(); i != destinations_set.end(); i++)
		{
			if (visited.find(i->first) != visited.end())
				continue;
			//List system as visited and point to the location we came from
			int cWeight = calculateWeight(*i, avoid); //Weight based on connection type
			q.push(std::make_pair(q.top().first + cWeight, std::make_pair(i->first,q.top().second.first)));
		}
		q.pop();
	}	

	return std::vector<int>(0);
finish:
	return path;
}

int Connections::calculateWeight(std::pair<int, std::set<std::string> > connection, std::set<std::string> avoidTags)
{
	for ( std::set<std::string>::iterator i = avoidTags.begin(); i != avoidTags.end(); i++)
	{
		if (connection.second.find(*i) != connection.second.end())
		{
			//If we want to avoid frigate holes, chances are we cant fit through there so make it weigh lots
			if (*i == "Frigate")
				return AVOID_WEIGHT * AVOID_WEIGHT;
			else
				return AVOID_WEIGHT;
		}
	}
	
	return 1;
}

std::set<std::string> Connections::getConnectionType(int from, int to)
{
	std::map<int, std::map<int,  std::set<std::string> > >::iterator i;
	std::map<int,  std::set<std::string> >::iterator j;
	i = connections.find(from);
	if (i != connections.end())
	{
		j = i->second.find(to);
		if (j != i->second.end())
			return j->second;
		else
			return std::set<std::string>();
	}
	else
		return std::set<std::string>();
}
