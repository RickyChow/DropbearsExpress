#ifndef SIGGYLOADER_H
#define SIGGYLOADER_H

#include "Vertex.h"
#include "IConnectionLoader.h"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "sqlite3.h"
#include "json_spirit.h"
#include "SystemInfo.h"
 

class SiggyLoader : public IConnectionLoader
{

public:
    SiggyLoader(SystemInfo* i, std::string request);
    void loadData(std::string request_str);
protected:
    std::string request;
    json_spirit::mValue siggyData;
    SystemInfo* info;
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string> split(const std::string &s, char delim);

};

#endif
