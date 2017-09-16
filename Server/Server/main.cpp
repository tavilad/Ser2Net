#define _CRT_SECURE_NO_WARNINGS
#include "Server.h"
#include "Serial.h"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <string>
#include <set>

using boost::asio::ip::tcp;
using namespace boost;

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: async_tcp_echo_server <port>\n";
			return 1;
		}

		boost::asio::io_service io_service;

		Server s(io_service, std::atoi(argv[1]));

		//serial.write();

		std::cout << "Welcome to the server!" << std::endl;

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}