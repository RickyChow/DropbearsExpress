//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

std::vector<std::string> stringsplit(const std::string &s, char delim, std::vector<std::string> &elems);
std::string getData();

std::string getData()
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

		std::string messageBody = "systemID=31002487&lastUpdate=0&mapOpen=true&mapLastUpdate=0&forceUpdate=true";

		boost::asio::streambuf request;

		std::ostream ss(&request);
		ss << "POST /siggy/siggy HTTP/1.1\r\n";
		ss << "Host: siggy.borkedlabs.com\r\n";
		ss << "Content-Length: " <<messageBody.size() << "\r\n";
		ss << "Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n";
		//ss << "Cookie: userID=d78ea4a2d7b9f4fc313984472276a4b418a080bf%7E17772; passHash=38a86601e3dfb720a6a5487fd30270fa0663a8c2%7E3f1c2f9824d2ee4bbe52b1b11d376fb29af6ad50bd515fe536b30782dde9fbd6;\r\n";
		ss << "Cookie:  userID=dff5423eabcf81438688b6e38f4a7e07d93ddf3e%7E17772; passHash=2f3d2c36c051322bef4671b3c6ea963e5235f715%7E3f1c2f9824d2ee4bbe52b1b11d376fb29af6ad50bd515fe536b30782dde9fbd6;\r\n";
		ss << "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.76 Safari/537.36\r\n";
		ss << "Connection: keep-alive\r\n";
		ss << "Cache-Control: no-cache\r\n\r\n";
		ss << messageBody;

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
	      return std::string();
	    }
	    if (status_code != 200)
	    {
	      std::cout << "Response returned with status code " << status_code << "\n";
	      //return 1;
	    }
	    // Read the response headers, which are terminated by a blank line.
		boost::asio::read_until(sock, response, "\r\n\r\n");

		// Process the response headers.
		std::string headerLine;
		std::stringstream header;
		uint contentLength = 0;
		while (std::getline(response_stream, headerLine) && headerLine != "\r")
		{
			std::vector<std::string> strElems;
			stringsplit(headerLine, ':', strElems);
			if (*strElems.begin() == "Content-Length")
				contentLength = std::atoi((strElems.rbegin())->c_str());
			header << headerLine;
		}

		// Read until EOF, writing data to output as we go.
		std::stringstream data_ss;
		std::string data;
		boost::system::error_code error;
		while (boost::asio::read(sock, response, boost::asio::transfer_at_least(1), error))
		{
			std::cout << &response;
			data_ss << &response;
			if (data_ss.str().size() >= contentLength)
				break;
		}

		if (error != boost::asio::error::eof)
		  throw boost::system::system_error(error);

		return data_ss.str();

	}
	catch(std::exception &e)
	{
		std::cout << e.what();
	}

	return std::string();
}

std::vector<std::string> stringsplit(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
