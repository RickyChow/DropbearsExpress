#ifndef ROUTEFINDER_H
#define ROUTEFINDER_H 

#include <vector>
#include <algorithm>
#include <iostream>

#include "router/RouteQueue.h"
#include "router/Vertex.h"
#include "loaders/IConnectionLoader.h"

#define BASE_WEIGHT 1
#define AVOID_WEIGHT 100


//Routefinder finds the optimal route in a map given a
//vector of loaders. Each loader populates the map with
//connections and new types of loaders can be created by
//inheriting the IConnectionLoader interface.
class RouteFinder 
{
public:
    //Initialise route finder with a vector of loaders:
    //loaders will populate the vertex map with 
    //up-to-date map info
    RouteFinder(std::vector<IConnectionLoader*>& loaders);

    //Findroute will find the shortest path between start
    //and finish based on the vertex map. 
    std::vector<int> findRoute(int start, int finish, TypeSet& avoid, std::set<int> ignore);
    std::vector<int> findRoute(int start, int finish);

private:
    std::vector<IConnectionLoader*> connectionLoaders;
    VertexMap vertexMap; 
	std::map<int, int> visited; 
    TypeSet avoid;
    
    //Adds neighbours of the curr sys into the queue
    //neighbours are contained in destination set
    void addNeighbours(RouteQueue& q, AdjacentsMap& destinations_set, int currSys);

    //Returns the weight of the connection given the avoid tags
    int calculateWeight(std::pair<int, TypeSet> connection, TypeSet& avoidTags);

    //Returns a vector containing the shortest path from start to finish
    //given a visited map which contains start and finish
    std::vector<int> getPath(int start, int finish);
};


#endif
