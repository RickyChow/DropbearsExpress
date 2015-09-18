#ifndef DBLoader_H
#define DBLoader_H

#include "IConnectionLoader.h"
#include "Vertex.h"
#include "sqlite3.h"
#include "SystemInfo.h"

class DBLoader : public IConnectionLoader
{
private:
    sqlite3* db;
    SystemInfo* info;

public:
    DBLoader(std::string path, SystemInfo* i);
    void loadConnection(VertexMap&);

};


#endif
