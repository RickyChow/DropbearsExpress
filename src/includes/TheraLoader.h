#ifndef THERALOADER_H
#define THERALOADER_H

#include "IConnectionLoader.h"

class TheraLoader : public IConnectionLoader
{
public:
    void loadConnection(std::set<Vertex>& vertices);

};

#endif
