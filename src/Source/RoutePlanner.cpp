// RoutePlanner.cpp : Defines the entry point for the console application.
//
#define SQLITE_MAX_COLUMN 20000
#define FCGI_MODE


#include "Connections.h"
#include "SystemInfo.h"
#include "Printing.h"
#include "Misc.h"
#include "ErrorHandler.h"


#include <fcgio.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

void FCGI(int, char*[]);
int parseURI(std::string in,SystemInfo& info, std::string& start, std::string& finish, std::set<std::string>& avoid);

std::string hendl = "<br>";
bool loadSiggy = true;

int main(int argc, char* argv[])
{
	if (argc > 1)
		loadSiggy = false;
	signal(SIGSEGV, handler);
	FCGI(argc, argv);



	return 0;
}

void FCGI(int argc, char* argv[])
{
	using namespace std;
	int requestCount = 0;
	std::string fName = "universeDataDx.db";

	if (loadSiggy)
		std::cout <<"Dropbears Express start\n";
	else
		std::cout <<"Dropbears express start - no siggy\n";

	std::string programPath = argv[0];
	std::string programDirectory = programPath.substr(0, programPath.find_last_of("/"));
	std::string dbPath = programDirectory + "/" + fName;
	std::string start, finish;
	std::set<std::string> avoidSet, ignoreSet;

	Siggy s = Siggy();
	SystemInfo info = SystemInfo(dbPath.c_str());
	info.updateWithSiggy(s);
	Connections c = Connections(dbPath.c_str(), &info);


	std::cout << "Ready!\n";



    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while (FCGX_Accept_r(&request) == 0)
    {

    	start = "";
    	finish = "";
    	avoidSet.clear();

    	const char * uri = FCGX_GetParam("REQUEST_URI", request.envp);

    	std::string uri_s = std::string(uri);
    	if (uri_s == "/favicon.ico")
    		continue;

        if (!parseURI(uri,info, start, finish, avoidSet))
        {
			std::cout << "START Request number: " << ++requestCount << "\n";
			std::cout << start << " to " << finish << "\n";
			c.clearAllConnections();
			std::cout << "Update\n";
			s.update();
			std::cout << "Load connections\n";
			c.loadConnections(dbPath.c_str());
			std::cout << "Updatewithsiggy\n";
			info.updateWithSiggy(s);
			if (loadSiggy)
			{
				std::cout << "loadsiggyconnections\n";
				c.loadSiggyConnections(s);
			}
			std::cout << "loadtheraconnections\n";
			c.loadTheraConnections(s);
			std::cout << "FINISH LOADING\n\n";
        }else
        {
        	std::cout << "Invalid inputs recieved\n\n";

        }

        // Backup the stdio streambufs
        streambuf * cin_streambuf  = cin.rdbuf();
        streambuf * cout_streambuf = cout.rdbuf();
        streambuf * cerr_streambuf = cerr.rdbuf();

        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);

        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);


        printHead();

        printForm();
        printRoute(c, info, start, finish, avoidSet, std::set<std::string>());
        std::cout << "</div>";



        FCGX_Finish_r(&request);

        // Note: the fcgi_streambuf destructor will auto flush
        // restore stdio streambufs
        cin.rdbuf(cin_streambuf);
        cout.rdbuf(cout_streambuf);
        cerr.rdbuf(cerr_streambuf);


    }
}



int parseURI(std::string in,SystemInfo& info,  std::string& start, std::string& finish, std::set<std::string>& avoid)
{
	std::vector<std::string> args;
	in.erase(0,2);
	args = Misc::split(in, '&');
	std::map<std::string, std::string> avoidMap;


	for (std::vector<std::string>::iterator i = args.begin(); i!= args.end(); i++)
	{
		std::vector<std::string> param = Misc::split(*i, '=');
		if (param.size() != 2)
			continue;
		if (param[0] == "start")
			start = param[1];
		else if (param[0] == "finish")
			finish = param[1];

		avoidMap.insert(std::make_pair(param[0], param[1]));

	}
	avoid =  Misc::getAvoidSet(avoidMap);


	if (start.length() <= 0 || finish.length() <= 0)
	{
		return -1;
	}


	if (info.getSID(start) == -1 || info.getSID(finish) == -1)
		return -1;


	return 0;




}


