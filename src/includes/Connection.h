#ifndef CONNECTION_H
#define CONNECTION_H

#include <set>
#include <string>

typedef std::set<std::string> ConnectionTypes;

class Connection{
public:
    int from, to;
    ConnectionTypes types;
    Connection(int f, int t, ConnectionTypes tp);
    bool operator<(const Connection& other) const;
    bool operator==(const Connection& other) const;
};

#endif
