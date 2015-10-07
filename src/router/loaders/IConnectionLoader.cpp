#include "loaders/IConnectionLoader.h"

void IConnectionLoader::addConnection(
        VertexMap& vertices, SystemTypeGetter* info,
        int from, int to)
{
    TypeSet tags = TypeSet();
    addConnection(vertices, info, from, to, tags);
}

void IConnectionLoader::addConnection(
        VertexMap& vertices, SystemTypeGetter* info,
        int from, int to, TypeSet tags)
{
	TypeSet toType, fromType;

	toType.insert(info->getType(to));
	toType.insert(tags.begin(), tags.end());
	fromType.insert(info->getType(from));
	fromType.insert(tags.begin(), tags.end());

    //Add vertices if they dont exists already
    //this will fail if it already exists
    vertices.insert(std::make_pair(from, Vertex(from))); 
    vertices.insert(std::make_pair(to, Vertex(to)));

    //Add edges to the vertices
    vertices.find(from)->second.addEdge(to, toType);
	vertices.find(to)->second.addEdge(from, fromType);
}
