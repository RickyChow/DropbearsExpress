#include "router/RouteFinder.h"
#include <iostream>

RouteFinder::RouteFinder(std::vector<IConnectionLoader*>& loaders)
   : vertexMap()
{
    for (int i=0; i<loaders.size(); ++i)
    {
        loaders[i]->loadConnection(vertexMap);
    }
}

std::vector<int> RouteFinder::findRoute(int start, int finish, TypeSet avoid, std::set<int> ignore)
{
    //Reverse priority queue for pair<distance, <systemID,SystemFrom>>
	RouteQueue q;

    //Contains <system visited, where we came from to get to system visited>
	std::map<int, int> visited; 
	std::vector<int> path;

    q.insertNext(0,start,start);
	
	if (start==finish)
	{
		path.push_back(start);
		return path;
	}
	while(!q.empty())
	{
		//Keep popping stuff from the queue if we've been to the next point already
        //, Otherwise add it to the visited set
		while (!visited.insert(std::make_pair(q.getNextSystem(), q.getNextFromSystem())).second)
		{
			q.pop();
			if (q.empty())
				return std::vector<int>(0);
		}

        int currSys = q.getNextSystem();
        int fromSys = q.getNextFromSystem();

		if (currSys == finish)
		{
			visited.insert(std::make_pair(finish, fromSys));
			path.push_back(finish);
			do
			{
				path.push_back(visited.find(path.back())->second);
			} while (path.back() != start);

			std::reverse(path.begin(), path.end());
		    return path;
		}
		//Set of destinations from the current point
		//AdjacentsMap destinations_set = connections.find(q.top().second.first)->second;
        AdjacentsMap destinations_set = vertexMap.find(currSys)->second.tos;

		//Iterate through all connected systems
        AdjacentsMap::iterator i;
		for (i = destinations_set.begin(); i != destinations_set.end(); i++)
		{
            int nextSys = i->first;
			if (visited.find(nextSys) != visited.end())
				continue;
			//List system as visited and point to the location we came from
			int cWeight = calculateWeight(*i, avoid); //Weight based on connection type
            q.insertNext(q.getNextDistance() + cWeight, nextSys, currSys); 
		}
		q.pop();
	}	

	return std::vector<int>(0);

}
int RouteFinder::calculateWeight(std::pair<int, TypeSet> connection, TypeSet avoidTags)
{
    TypeSet::iterator i;
	for (i = avoidTags.begin(); i != avoidTags.end(); i++)
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
