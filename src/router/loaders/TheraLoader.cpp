#include "loaders/TheraLoader.h"
TheraLoader::TheraLoader(SystemInfo* i, std::string s)
    :SiggyLoader(i,s)
{
}

std::string TheraLoader::loadTheraRequest()
{
	std::stringstream ss;

	ss << "GET /thera/latest_exits? HTTP/1.1\r\n";
	ss << "Host: siggy.borkedlabs.com\r\n";
	ss << "Accept: application/json, text/javascript, */*; q=0.01\r\n";
	ss << "Content-Encoding: \r\n";
	ss << "Cookie: userID=d78ea4a2d7b9f4fc313984472276a4b418a080bf%7E17772; passHash=38a86601e3dfb720a6a5487fd30270fa0663a8c2%7E3f1c2f9824d2ee4bbe52b1b11d376fb29af6ad50bd515fe536b30782dde9fbd6;\r\n";
	ss << "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.76 Safari/537.36\r\n";
	ss << "Connection: keep-alive\r\n";
	ss << "Cache-Control: no-cache\r\n\r\n";

	return std::string(ss.str());


}

void TheraLoader::loadConnection(VertexMap& vertices)
{
    loadData(loadTheraRequest());
	json_spirit::mValue val = siggyData;
	json_spirit::mObject wormHoles_obj = val.get_obj();
	for (json_spirit::mObject::iterator i = wormHoles_obj.begin(); i != wormHoles_obj.end(); i++)
	{
		//std::cout << i->first << std::endl;
		int to_ws = i->second.get_obj().find("system")->second.get_obj().find("id")->second.get_int();

		//Add connection to thera
		 std::set<std::string> tags;
		tags.insert("Thera");
		tags.insert("JS");
		tags.insert("WH");
		//addConnections(31000005, to_ws, tags);
        int THERA_ID = 31000005;
        addConnection(vertices, info, THERA_ID, to_ws, tags);
	}
}
