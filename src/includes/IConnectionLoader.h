#ifndef ICONNECTIONLOADER_H
#define ICONNECTIONLOADER_H

#include <set>
#include "Vertex.h"
#include "SystemInfo.h"

class IConnectionLoader
{
public:
    virtual void loadConnection(VertexMap&)=0;
protected:
    void addConnection(VertexMap& vertices, SystemInfo* info,
                                     int from, int to, TypeSet tags);
};

#endif
