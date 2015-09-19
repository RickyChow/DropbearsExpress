#ifndef VERTEX_H
#define VERTEX_H

#include <set>
#include <string>
#include <map>

typedef std::set<std::string> TypeSet;
typedef std::map<int, TypeSet> AdjacentsMap;

class Vertex{
public:
    int from;
    AdjacentsMap tos;
    Vertex(int f, AdjacentsMap a);
    Vertex(int f);
    void addEdge(int, TypeSet);
};

typedef std::map<int, Vertex> VertexMap;

#endif
