#ifndef THERALOADER_H
#define THERALOADER_H

#include "loaders/SiggyLoader.h"

class TheraLoader : public SiggyLoader 
{
public:
    TheraLoader(SystemInfo*, std::string);
    void loadConnection(VertexMap& vertices);
private:
    std::string loadTheraRequest();
};

#endif
