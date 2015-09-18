#include "NovaLoader.h"
NovaLoader::NovaLoader(SystemInfo* i, std::string s)
    :SiggyLoader(i, s)
{}

std::string NovaLoader::loadNovaRequest()
{

	std::string messageBody = "systemID=31002487&lastUpdate=0&mapOpen=true&mapLastUpdate=0&forceUpdate=true";

	std::stringstream ss;
	ss << "POST /siggy/siggy HTTP/1.1\r\n";
	ss << "Host: siggy.borkedlabs.com\r\n";
	ss << "Connection: keep-alive\r\n";
	ss << "Accept: application/json, text/javascript, */*; \r\n";
	ss << "Content-Length: " <<messageBody.size() << "\r\n";
	ss << "Content-Encoding: \r\n";
	ss << "Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n";
	ss << "Cookie: userID=d78ea4a2d7b9f4fc313984472276a4b418a080bf%7E17772; passHash=38a86601e3dfb720a6a5487fd30270fa0663a8c2%7E3f1c2f9824d2ee4bbe52b1b11d376fb29af6ad50bd515fe536b30782dde9fbd6;\r\n";
	ss << "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.76 Safari/537.36\r\n";
	ss << "\r\n";
	ss << messageBody;


	return std::string(ss.str());
}

void NovaLoader::loadConnection(VertexMap& vertices)
{
    
    loadData(loadNovaRequest());
	json_spirit::mValue val = siggyData;
	json_spirit::mObject chainMap_obj = val.get_obj().find("chainMap")->second.get_obj();

	if (chainMap_obj.find("wormholes")->second.type() == json_spirit::obj_type)
	{
		json_spirit::mObject wormHoles_Obj = chainMap_obj.find("wormholes")->second.get_obj();
		for (json_spirit::mObject::iterator i = wormHoles_Obj.begin(); i != wormHoles_Obj.end(); i++)
		{
			 std::set<std::string> tags;
			tags.insert("WH");
			json_spirit::mObject connectionObj = i->second.get_obj();
			std::string from_ws = connectionObj.find("from_system_id")->second.get_str();
			std::string to_ws = connectionObj.find("to_system_id")->second.get_str();

			if (connectionObj.find("frigate_sized")->second.get_str() == "1")
				tags.insert("Frigate");
			if (connectionObj.find("eol")->second.get_str() == "1")
				tags.insert("EOL");
			if (connectionObj.find("mass")->second.get_str() == "1")
				tags.insert("Reduced");
			else if (connectionObj.find("mass")->second.get_str() == "2")
				tags.insert("Crit");

            int from = atoi((char*)from_ws.c_str());
            int to = atoi((char*)to_ws.c_str());
            addConnection(vertices, info, from, to, tags);
		}
	}
	if (chainMap_obj.find("jumpbridges")->second.type() == json_spirit::obj_type)
	{
		json_spirit::mObject jumpBridge_Obj = chainMap_obj.find("jumpbridges")->second.get_obj();
		for (json_spirit::mObject::iterator i = jumpBridge_Obj.begin(); i != jumpBridge_Obj.end(); i++)
		{
			json_spirit::mObject connectionObj = i->second.get_obj();
			std::string from_ws = connectionObj.find("from_system_id")->second.get_str();
			std::string to_ws = connectionObj.find("to_system_id")->second.get_str();

			 std::set<std::string> tags;
			tags.insert("JB");

            int from = atoi((char*)from_ws.c_str());
            int to = atoi((char*)to_ws.c_str());
            addConnection(vertices, info, from, to, tags);
		}
	}
}
