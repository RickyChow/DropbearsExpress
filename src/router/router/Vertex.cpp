#include "router/Vertex.h"

Vertex::Vertex(int f, AdjacentsMap a)
    :from(f), tos(a)
{
} 
Vertex::Vertex(int f)
    :from(f), tos()
{
}

void Vertex::addEdge(int to, TypeSet types)
{
    tos.insert(std::make_pair(to, types));
}
