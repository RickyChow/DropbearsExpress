#include "loaders/DBLoader.h"

DBLoader::DBLoader(std::string path, SystemInfo* i)
{
    sqlite3_open(path.c_str(), &db);
    info = i;
}
void DBLoader::loadConnection(VertexMap& vertices)
{
	sqlite3_stmt *jumpStatement;
	sqlite3_prepare_v2(db, "SELECT * from mapSolarSystemJumps", -1, &jumpStatement, 0);

	while(sqlite3_step(jumpStatement) == SQLITE_ROW)
	{
		int fromSysID = sqlite3_column_int(jumpStatement, 2);
		int toSysID = sqlite3_column_int(jumpStatement, 3);

		//Try to insert the start connection, if it doesn't exist, then create an empty set
		std::set<std::string> tags;
		tags.insert("Gate");
		addConnection(vertices, info,  fromSysID, toSysID, tags);
	}
}
