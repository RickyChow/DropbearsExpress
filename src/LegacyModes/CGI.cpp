#include "CGI.h"
#include "Printing.h"


void CGI(int argc, char* argv[], std::string dbPath)
{
	using namespace std;
	std::string hendl = "<br>";

	std::map<std::string, std::string> Get;
	std::set<std::string> avoidSet;

	std::string programPath = argv[0];
	std::string programDirectory = programPath.substr(0, programPath.find_last_of("/"));
	dbPath = programDirectory + "/" + dbPath;



	initializeGet(Get); //notice that the variable is passed by reference!

	cout<<"Content-type: text/html"<<endl<<endl;
	cout<<"<html><body>";
	cout<<"<h1>Dropbears Express - InternetHero</h1>";
	cout<<"<form method=\"get\">";
	cout<<" <label for=\"start\">Starting system: </label>";
	cout<<" <input type=\"text\" name=\"start\" id=\"start\">"<< hendl;
	cout<<" <label for=\"finish\">Finish system: </label>";
	cout<<" <input type=\"text\" name=\"finish\" id=\"finish\">"<< hendl << hendl;
	cout<<" Avoid:" << hendl;
	cout<<"  <input type=\"checkbox\" name=\"frigate\" value=\"Frigate\"> Frigate hole";
	cout<<"  <input type=\"checkbox\" name=\"reduced\" value=\"Reduced\"> Reduced hole";
	cout<<"  <input type=\"checkbox\" name=\"crit\" value=\"Crit\"> Crit hole";
	cout<<"  <input type=\"checkbox\" name=\"eol\" value=\"EOL\"> EOL hole" << hendl;
	cout<<"  <input type=\"checkbox\" name=\"gate\" value=\"Gate\"> Gates";
	cout<<"  <input type=\"checkbox\" name=\"wh\" value=\"WH\"> Wormholes";
	cout<<"  <input type=\"checkbox\" name=\"jb\" value=\"JB\"> Jumpbridges" << hendl;
	cout<<"  <input type=\"checkbox\" name=\"hs\" value=\"HS\"> HS";
	cout<<"  <input type=\"checkbox\" name=\"ls\" value=\"LS\"> LS";
	cout<<"  <input type=\"checkbox\" name=\"ns\" value=\"NS\"> NS";
	cout<<"  <input type=\"checkbox\" name=\"js\" value=\"JS\"> JS";
	cout<<"  <input type=\"checkbox\" name=\"thera\" value=\"Thera\"> Thera" << hendl << hendl;

	avoidSet = Misc::getAvoidSet(Get);

	cout<<" <input type=\"submit\"/>"<<endl;
	cout<<"</form><br /><br />"<<endl;

	routePlannerCGI(dbPath, Get["start"], Get["finish"], avoidSet);

	cout<<"</body></html>"<<endl;

}






void routePlannerCGI(std::string path, std::string start, std::string finish, std::set<std::string> avoid)
{
	Siggy s = Siggy();
	SystemInfo info = SystemInfo(path.c_str());
	info.updateWithSiggy(s);
	Connections c = Connections(path.c_str(), &info);
	c.loadSiggyConnections(s);
	c.loadTheraConnections(s);

	printRoute(c, info, start, finish, avoid, std::set<std::string>());

}
