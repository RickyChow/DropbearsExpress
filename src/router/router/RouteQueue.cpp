#include "router/RouteQueue.h"


int RouteQueue::getNextSystem()
{
    return top().second.first;
}
int RouteQueue::getNextFromSystem()
{
    return top().second.second;
}
int RouteQueue::getNextDistance()
{
    return top().first;
}
void RouteQueue::insertNext(int distance, int next, int prev)
{
    push(std::make_pair(distance, std::make_pair(next,prev)));
}
