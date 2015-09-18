#ifndef NOVALOADER_H
#define NOVALOADER_H

#include "SiggyLoader.h"


class NovaLoader : public SiggyLoader
{
public:
    NovaLoader(SystemInfo*, std::string);
    void loadConnection(VertexMap&);
private:
    std::string loadNovaRequest();
};


#endif
