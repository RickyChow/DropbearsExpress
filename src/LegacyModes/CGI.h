#include <iostream>
#include <set>
#include <map>
#include <fcgio.h>
#include "Siggy.h"
#include "Connections.h"
#include "getpost.h"
#include "Misc.h"
#include "Printing.h"

void CGI(int, char*, std::string);

void routePlannerCGI(std::string path, std::string start, std::string finish, std::set<std::string> avoid);
