#ifndef SIGGYLOADER_H
#define SIGGYLOADER_H

#include "Vertex.h"
#include "IConnectionLoader.h"

class SiggyLoader : public IConnectionLoader
{

public:
    void loadConnection(VertexMap&);

};

#endif
