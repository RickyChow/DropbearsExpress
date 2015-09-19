#ifndef DBLoader_H
#define DBLoader_H

#include "loaders/IConnectionLoader.h"
#include "router/Vertex.h"
#include "sqlite3/sqlite3.h"
#include "utils/SystemInfo.h"

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
