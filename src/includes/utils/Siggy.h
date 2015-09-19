#pragma once

#ifndef SIGGY_H
#define SIGGY_H

#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>


#include "sqlite3/sqlite3.h"
#include "json_spirit/json_spirit.h"

class Siggy
{
	std::string novaHeaderFilepath, theraHeaderFilepath;
	std::string novaRequestData, theraRequestData;
	std::wstring novaURL, theraURL;
	json_spirit::mValue novaData, theraData;

	std::string loadNovaRequest();
	std::string loadTheraRequest();

public:
	Siggy();
	void update();
	json_spirit::mValue getData(std::string);
	json_spirit::mValue getNovaData();
	json_spirit::mValue getTheraData();
};

#endif
