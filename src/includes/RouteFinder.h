#ifndef ROUTEFINDER_H
#define ROUTEFINDER_H 

#include "Vertex.h"
#include "IConnectionLoader.h"
#include <vector>
#include <queue>
#include <algorithm>

#define AVOID_WEIGHT 100

typedef std::pair<int,std::pair<int,int> > QElement;
typedef	std::priority_queue<QElement,std::vector<QElement>, std::greater<QElement> > RouteQ;

class RouteQueue : public RouteQ
{
public:
    int getNextSystem(){return top().second.first;}
    int getNextFromSystem(){return top().second.second;}
    int getNextDistance(){return top().first;}
    void insertNext(int distance, int next, int prev){
        push(std::make_pair(distance, std::make_pair(next,prev)));
    }
};

class RouteFinder 
{
public:
    RouteFinder(std::vector<IConnectionLoader*>& loaders);
    std::vector<int> findRoute(int start, int finish, std::set<std::string> avoid, std::set<int> ignore);
private:
    VertexMap vertexMap; 
    std::vector<IConnectionLoader*> connectionLoaders;
    int calculateWeight(std::pair<int, std::set<std::string> > connection, std::set<std::string> avoidTags);
};


#endif
