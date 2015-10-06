#include "router/RouteFinder.h"

RouteFinder::RouteFinder(std::vector<IConnectionLoader*>& loaders)
   : vertexMap()
{
    for (int i=0; i<loaders.size(); ++i)
        loaders[i]->loadConnection(vertexMap);
}

//Finds the best route from start to finish in vertexMap while avoiding
//connections type defined in avoid
std::vector<int> RouteFinder::findRoute(
        int start, int finish, TypeSet& avoid, std::set<int> ignore)
{
    //Reverse priority queue for pair<distance, <systemID,SystemFrom>>
	RouteQueue q;
    this->avoid = avoid;

    visited.clear();
    q.insertNext(0,start,start);
	
	if (start==finish)
        return std::vector<int>(1, start);

    //The queue contains systems to visit in order of closeness to start
	while(!q.empty())
	{
		//Keep popping stuff from the queue if we've been to the next 
        //point already, Otherwise add it to the visited set
		while (!visited.insert(std::make_pair(q.getNextSystem(), q.getNextFromSystem())).second)
		{
			q.pop();
            //We've exhausted the queue, destination non reachable
			if (q.empty())
				return std::vector<int>(0);
		}

        int currSys = q.getNextSystem();
        int fromSys = q.getNextFromSystem();

        //We've found the destination
		if (currSys == finish)
		    return getPath(start, finish);
		else
        {
		    //Get the set of destinations from the current point
            AdjacentsMap destinations_set = vertexMap.find(currSys)->second.tos;
            addNeighbours(q, destinations_set, currSys);
		    q.pop();
        }
	}	

    //Destination was never reached: return empty path
	return std::vector<int>(0);

}

//Adds neighbours of the current system into the route queue
void RouteFinder::addNeighbours(
        RouteQueue& q, AdjacentsMap& destinations_set, int currSys)
{
	//Iterate through all connected systems
    AdjacentsMap::iterator i;
	for (i = destinations_set.begin(); i != destinations_set.end(); i++)
	{
        int nextSys = i->first;
		if (visited.find(nextSys) != visited.end())
			continue;
		int cWeight = calculateWeight(*i, avoid); 
        q.insertNext(q.getNextDistance() + cWeight, nextSys, currSys); 
	}
}

//Gets path from visited set. Function starts from the finish and backtracks
//until it finds the start then returns a vector of systemIDs
std::vector<int> RouteFinder::getPath(int start, int finish)
{
    std::vector<int> path;
	path.push_back(finish);
	do
	{
        //Use the second value of visited pair to find the next system
		path.push_back(visited.find(path.back())->second);
	} while (path.back() != start);

	std::reverse(path.begin(), path.end());
    return path;
}

//Works out the weight of the current connection based on the input tags
int RouteFinder::calculateWeight(std::pair<int, TypeSet> connection, TypeSet& avoidTags)
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
	
	return BASE_WEIGHT;
}
