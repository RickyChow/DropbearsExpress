#ifndef ICONNECTIONLOADER_H
#define ICONNECTIONLOADER_H

#include <set>
#include "router/Vertex.h"
#include "utils/SystemInfo.h"

class IConnectionLoader
{
public:
    virtual void loadConnection(VertexMap&)=0;
protected:
    void addConnection(VertexMap& vertices, SystemInfo* info,
                                     int from, int to, TypeSet tags);
};

#endif
