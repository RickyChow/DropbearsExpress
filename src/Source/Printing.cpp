#include "Printing.h"


int printRoute(Connections& c, SystemInfo& info, std::string start, std::string finish, std::set<std::string> avoidSet, std::set<std::string> ignore)
{

	std::set<int> ignore_int;

	if (info.getSID(start) == -1 || info.getSID(finish) == -1)
	{
		std::cout << "Invalid system name" << std::endl;
		return -1;
	}

	for (std::set<std::string>::iterator i = ignore.begin(); i!=ignore.end(); i++)
	{
		ignore_int.insert(info.getSID(*i));
	}


	std::vector<int> path = c.getRouteDijkstra(info.getSID(start), info.getSID(finish), avoidSet, ignore_int);

	if (path.empty())
	{
		std::cout << "Destination not reachable" << std::endl;
		return 0;
	}

	std::cout << "<table>"
			<<"<thead>"
			<<"<tr>"
			<<"<th class=\"ResultHeader\">Security status</th> <th class=\"ResultHeader\">Connection Type</th>"
			<<"<th class=\"ResultHeader\">System</th><th class=\"ResultHeader\">Siggy name</th>"
			<<"</tr>"
			<<"</thead>"
			<<"<tbody>";
	for (std::vector<int>::iterator i = path.begin(); i != path.end(); i++)
	{
		std::cout << "<tr bgcolor=\"" << getHTMLColour(info, *i) << "\">";

		std::stringstream connectionType;
		if (i != path.begin())
		{
			std::vector<int>::iterator prevItr = --i;
			i++;
			std::set<std::string> types = c.getConnectionType(*prevItr, *i);
			for (std::set<std::string>::iterator j = types.begin(); j != types.end(); j++)
			{
				if (*j != "HS" && *j != "LS" && *j!= "NS" && *j != "JS" && *j != "Thera")
					connectionType  << *j;
			}
		}
		//std::cout << "[" << info.getType(*i) << "]" << connectionType.str() << info.getSName(*i) << " " << info.getNickname(*i) << " <br>" << std::endl;

		std::cout << "<td class=\"SecStatus\">" << info.getSecurity(*i) << "</td>";
		std::cout << "<td>" << connectionType.str() << "</td>";
		std::cout << "<td>" << info.getSName(*i) << "</td>";
		std::cout << "<td>" << info.getNickname(*i) << "</td>";
		std::cout << "</tr>";
	}
	std::cout << "</tbody></table>";

	std::cout << "Jumps: " << path.size()-1 << std::endl;
	return 0;
}

std::string getHTMLColour(SystemInfo& info, int system)
{
	float sec = info.getSecurity(system);
	if (sec <= 0.0)
		return "#FF0000";
	else if (sec < 0.5)
		return "#FF8000";
	else
		return "#O1DF3A";

}

void printHead()
{
	std::string hendl = "<br>";
    std::cout << "Content-type: text/html\r\n"
         << "\r\n"
         << "<html>\n"
         << "  <head>\n"
         << "    <title>Dropbears Express</title>" << "\n"
         << "	 <link rel=\"stylesheet\" href=\"/css/stylesheet.css\" type=\"text/css\" />" << "\n"
         << "  </head>\n";


}

void printForm()
{
	std::string hendl = "<br>";

	using namespace std;
	cout<<  "  <body background=\"/images/background.jpg\">\n"
		 << "<div id=\"main\">"
         << "  		<div class=\"title\">\n"
         << "    		<h1><img src=\"/images/logo.png\"/>Dropbears Express</h1>\n"
    	 << "		</div>\n"
    	 << "		<div class=\"intro\">\n"
    	 << "			<p>If anything is broken please message InternetHero" << hendl
    	 << "			This router will pick the path with the least jumps given"
    	 << "			 a set of system types and connection types to avoid." << hendl
    	 << "			System names are not case sensitive." << hendl
    	 << "			Wormhole systems you can input as their nicknames are currently nova and thera. Otherwise you have to type in their proper names<p>"
    	 << "		</div>";

	cout<<"<form method=\"get\">";
	cout<<" <div class=\"TextBoxes\">";
	cout<<" <label for=\"start\">Starting system: </label>" << hendl;
	cout<<" <input type=\"text\" name=\"start\" id=\"start\">"<< hendl;
	cout<<" <label for=\"finish\">Finish system: </label>"<< hendl;
	cout<<" <input type=\"text\" name=\"finish\" id=\"finish\">"<< hendl;
	cout<<" </div>";
	cout<<" Avoid:" << hendl;
	cout<<" <table>";
	cout<<" <thead>"
			<<" <tr>"
			<<" 	<th> Wormhole status </th>"
			<<"		<th> Connection type </th>"
			<<"		<th> Security status </th>"
			<<" </tr>"
			<<" </thead>"
			<<" <tbody>"
			<<" <tr>"
			<<" 	<td><input type=\"checkbox\" name=\"frigate\" value=\"Frigate\"> Frigate hole</td>"
			<<" 	<td><input type=\"checkbox\" name=\"gate\" value=\"Gate\"> Stargate</td>"
			<<" 	<td><input type=\"checkbox\" name=\"hs\" value=\"HS\"> High Sec</td>"
			<<" </tr>"
			<<" <tr>"
			<<"		<td><input type=\"checkbox\" name=\"reduced\" value=\"Reduced\"> Reduced hole</td>"
			<<"		<td><input type=\"checkbox\" name=\"wh\" value=\"WH\"> Wormholes</td>"
			<<"		<td><input type=\"checkbox\" name=\"ls\" value=\"LS\"> Low Sec</td>"
			<<" </tr>"
			<<" <tr>"
			<<"		<td><input type=\"checkbox\" name=\"crit\" value=\"Crit\"> Crit hole</td>"
			<<"		<td><input type=\"checkbox\" name=\"jb\" value=\"JB\"> Jumpbridges</td>"
			<<"		<td><input type=\"checkbox\" name=\"ns\" value=\"NS\"> Null Sec</td>"
			<<" </tr>"
			<<" <tr>"
			<<"		<td><input type=\"checkbox\" name=\"eol\" value=\"EOL\"> EOL hole</td>"
			<<"		<td></td>"
			<<"		<td><input type=\"checkbox\" name=\"js\" value=\"JS\"> J-Space</td>"
			<<" </tr>"
			<<" <tr>"
			<<"		<td></td>"
			<<"		<td></td>"
			<<"		<td><input type=\"checkbox\" name=\"thera\" value=\"Thera\"> Thera</td>"
			<<" </tr>"

			<<" </tbody>";

	cout<<"</table>" << hendl << hendl;

	cout<<" <input type=\"submit\"/>"<<endl;
	cout<<"</form>"<<endl;
}
