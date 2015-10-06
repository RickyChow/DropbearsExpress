#ifndef ROUTEQUEUE_H
#define ROUTEQUEUE_H

#include <queue>


typedef std::pair<int,std::pair<int,int> > QElement;
typedef	std::priority_queue<QElement,
        std::vector<QElement>,
        std::greater<QElement> > RouteQ;

class RouteQueue : public RouteQ
{
public:
    int getNextSystem();
    int getNextFromSystem();
    int getNextDistance();
    void insertNext(int distance, int next, int prev);
};
#endif
