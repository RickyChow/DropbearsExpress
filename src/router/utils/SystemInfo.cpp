#include "utils/SystemInfo.h"

SystemInfo::SystemInfo(const char* fileName)
{
	sqlite3 *db;
	sqlite3_stmt *sNameStatement;

	sqlite3_open(fileName, &db);
	sqlite3_prepare_v2(db, "SELECT * from mapSolarSystems", -1, &sNameStatement, 0);
	while (sqlite3_step(sNameStatement) == SQLITE_ROW)
	{
		int sysID = sqlite3_column_int(sNameStatement, 2);
		std::string sysName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(sNameStatement, 3)));
		
		System s;
		s.sID = sysID;
		s.sName = sysName;
		s.security = floor((float)sqlite3_column_double(sNameStatement, 21) * 10.0 + 0.5)/10.0;
		
		if (s.sName == "Thera")
		{
			s.type = "Thera";
			Thera.insert(s.sID);
			WHsystems.insert(s.sID);
		}
		else if (s.security >= 0.5)
		{
			s.type = "HS";
			HSsystems.insert(s.sID);
		}
		else if (s.security > 0.0)
		{
			s.type = "LS";
			LSsystems.insert(s.sID);
		}
		else if (s.security > -1)
		{
			s.type = "NS";
			NSsystems.insert(s.sID);
		}
		else if (s.security >= -1 && s.sName.length() < 7) //to distinguish from j-space and k-space -1.0 systems
		{
			s.type = "NS";
			NSsystems.insert(s.sID);
		}
		else
		{
			s.type = "JS";
			WHsystems.insert(s.sID);
		}

		sID.insert(std::make_pair(Misc::lowerCase(sysName), sysID));
		systems.insert(std::make_pair(sysID, s));
	}
}

std::string SystemInfo::getSName(int id)
{
	if (systems.find(id) != systems.end())
		return systems.find(id)->second.sName;
	else
		return "";
}

void SystemInfo::updateWithSiggy(Siggy sData)
{
	json_spirit::mValue val = sData.getNovaData();
	json_spirit::mObject systems_Obj = val.get_obj().find("chainMap")->second.get_obj().find("systems")->second.get_obj();

	for (json_spirit::mObject::iterator i = systems_Obj.begin(); i != systems_Obj.end(); i++)
	{
		int sysID = std::atoi((char*)i->first.c_str());
		std::string nickname = i->second.get_obj().find("displayName")->second.get_str();
		systems.find(sysID)->second.nickname = std::string(nickname.begin(), nickname.end());
	}
}

int SystemInfo::getSID(std::string name)
{
	if (name.length() <= 0)
		return -1;

	std::string nameLower = Misc::lowerCase(name);


	if (nameLower == "nova")
		return sID.find("j105934")->second;

	if (sID.find(nameLower) != sID.end())
		return sID.find(nameLower)->second;
	else
		return -1;
}

float SystemInfo::getSecurity(int id)
{
	if (systems.find(id) != systems.end())
		return systems.find(id)->second.security;
	else
		return -100;
}

std::string SystemInfo::getType(int id)
{
	if (systems.find(id) != systems.end())
		return systems.find(id)->second.type;
	else
		return "";
}

std::string SystemInfo::getNickname(int id)
{
	if (systems.find(id) != systems.end())
		return systems.find(id)->second.nickname;
	else
		return "";
}

std::set<int> SystemInfo::getHS()
{
	return HSsystems;
}

std::set<int> SystemInfo::getLS()
{
	return LSsystems;
}

std::set<int> SystemInfo::getNS()
{
	return NSsystems;
}

std::set<int> SystemInfo::getWH()
{
	return WHsystems;
}
std::set<int> SystemInfo::getThera()
{
	return Thera;
}
