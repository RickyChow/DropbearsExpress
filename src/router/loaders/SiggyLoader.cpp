#include "loaders/SiggyLoader.h"

SiggyLoader::SiggyLoader(SystemInfo* i, std::string req)
    : info(i),
      request(req)
{}

void SiggyLoader::loadData(std::string request_str)
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

		  return;
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

		siggyData = outData;
        return;

	}
	catch(std::exception &e)
	{
		std::cout << e.what();
	}

	siggyData = json_spirit::mValue();
}

std::vector<std::string>& SiggyLoader::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> SiggyLoader::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

