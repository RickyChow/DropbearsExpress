#include "Siggy.h"

std::vector<std::string> split(const std::string &s, char delim);

Siggy::Siggy()
{
	novaURL = L"https://siggy.borkedlabs.com/update";
	theraURL = L"https://siggy.borkedlabs.com/thera/latest_exits?_=1426139077345";
	novaRequestData = "systemID=31002487&lastUpdate=0&group_cache_time=0&systemName=&freezeSystem=0&acsid=0&mapOpen=true&mapLastUpdate=0&forceUpdate=true";
	theraRequestData = "_=1426139077345";
	update();
}

void Siggy::update()
{
	std::cout << "Loading Nova connections\n\r";
	novaData = getData(loadNovaRequest());
	std::cout << "Loading thera connections \n\r";
	theraData = getData(loadTheraRequest());
}

std::string Siggy::loadNovaRequest()
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

std::string Siggy::loadTheraRequest()
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

json_spirit::mValue Siggy::getData(std::string request_str)
{
	using boost::asio::ip::tcp;
	namespace ssl = boost::asio::ssl;
	typedef ssl::stream<tcp::socket> ssl_socket;

	// Create a context that uses the default paths for
	// finding CA certificates.
	ssl::context ctx(ssl::context::sslv23);
	ctx.set_default_verify_paths();

	// Open a socket and connect it to the remote host.
	boost::asio::io_service io_service;
	ssl_socket sock(io_service, ctx);
	tcp::resolver resolver(io_service);
	tcp::resolver::query query("siggy.borkedlabs.com", "https");

	tcp::socket::lowest_layer_type& sockl = sock.lowest_layer();

	boost::asio::connect(sockl, resolver.resolve(query));

	sockl.set_option(tcp::no_delay(true));

	try{
		sock.set_verify_mode(ssl::verify_none);
		sock.handshake(ssl_socket::client);
		boost::asio::streambuf request;
		std::ostream ss(&request);
		ss << request_str;

		boost::asio::write(sock, request);
		boost::asio::streambuf response;
		boost::asio::read_until(sock, response, "\r\n");
		std::istream response_stream(&response);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		{
		  std::cout << "Invalid response\n";
		}
		if (status_code != 200)
		{
		  std::cout << "Response returned with status code " << status_code << "\n";

		  boost::asio::streambuf responseMsg;
		  boost::asio::read_until(sock, responseMsg, "\r\n\r\n");
		  std::istream message_stream(&responseMsg);
		  std::string errMsg_str;
		  message_stream >> errMsg_str;
		  std::cout << errMsg_str;

//		  std::string r_str;
//		  response_stream >> r_str;

		  return 1;
		}
		// Read the response headers, which are terminated by a blank line.
		boost::asio::read_until(sock, response, "\r\n\r\n");

		// Process the response headers.
		std::string headerLine;
		std::stringstream header;
		uint contentLength = 0;
		while (std::getline(response_stream, headerLine) && headerLine != "\r")
		{
			std::vector<std::string> strElems = split(headerLine, ':');
			if (*strElems.begin() == "Content-Length")
				contentLength = std::atoi((strElems.rbegin())->c_str());
			header << headerLine;
		}



		std::vector<char> data_vect;
		std::string data_read;
		std::string data_s;

		if (contentLength == 0) //Chunked data
		{
			std::stringstream message_ss;
			while(true)
			{
				unsigned int length;
				std::string length_s, line_s;
				std::stringstream line_ss, hex_ss;
				std::stringstream length_ss;

				//Get the length of the chunk
				boost::asio::read_until(sock, response, "\r\n");
				std::istream lengthResponse(&response);
				std::getline(lengthResponse, line_s);
				hex_ss << std::hex << line_s;
				hex_ss >> length;

				//If the length is 0 it means we're done
				if (length <= 0)
					break;

				//Read the chunk of data
				boost::asio::read_until(sock, response, "\r\n");
				std::istream lineResponse(&response);
				std::getline(lineResponse, line_s);

				//Stream the line into the stringstream
				message_ss << line_s;
			}

			//Pull stringstream data into a string, remove newlines
			data_s = message_ss.str();
			data_s.erase(std::remove(data_s.begin(), data_s.end(), '\n'), data_s.end());
			data_s.erase(std::remove(data_s.begin(), data_s.end(), '\r'), data_s.end());

		}else //Not chunked data
		{
			//std::cout << "NOT CHUNKED DATA, content length is" << contentLength << "\n";;
			while (boost::asio::read(sock, response, boost::asio::transfer_at_least(contentLength)))
			{
				std::stringstream data_ss;
				data_ss << &response;

				data_read = data_ss.str();
				int openBrackets = std::count(data_read.begin(), data_read.end(),'{');
				int closeBrackets = std::count(data_read.begin(), data_read.end(),'}');

				if (openBrackets > 0 && (openBrackets == closeBrackets))
				{
					break;
				}
			}
			//std::cout << data_read << "\n";
			data_s = data_read;
		}


		json_spirit::mValue outData;
		std::string::const_iterator data_s_begin = data_s.begin();
		json_spirit::read(data_s_begin, data_s.end(), outData);

//		//DEBUG
//		json_spirit::mObject mo = outData.get_obj();
//		for (json_spirit::mObject::iterator i = mo.begin(); i != mo.end(); i++)
//			std::cout << i->first << "\n";
//		json_spirit::mObject cm = mo.find("chainMap")->second.get_obj();
//		for (json_spirit::mObject::iterator i = cm.begin(); i != cm.end(); i++)
//			std::cout << i->first << "\n";
//		json_spirit::mObject wormHoles_Obj = cm.find("wormholes")->second.get_obj();

		return outData;

	}
	catch(std::exception &e)
	{
		std::cout << e.what();
	}

	return json_spirit::mValue();
}

json_spirit::mValue Siggy::getNovaData()
{
	return novaData;
}

json_spirit::mValue Siggy::getTheraData()
{
	return theraData;
}


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

