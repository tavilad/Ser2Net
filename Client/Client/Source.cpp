#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main()
{
	try
	{
		boost::asio::io_service io_service;
		char* serverName = "localhost";
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(serverName, "daytime");

		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_iterator);

		std::cout << "Introduceti comanda" << std::endl;
		std::cout << "1. Daytime" << std::endl;
		std::cout << "2. Hello" << std::endl;
		std::cout << "3. Connected clients ip" << std::endl;
		std::cout << "4. Write to serial port" << std::endl;
		std::cout << "0. Close client" << std::endl;
		int option;
		std::cin >> option;
		while (option != 0)
		{
			boost::array<char, 128> buf;
			boost::system::error_code ignored_error;

			switch (option)
			{
			case 1:
				boost::asio::write(socket, boost::asio::buffer("0"));
				break;
			case 2:
				boost::asio::write(socket, boost::asio::buffer("1"));
				break;
			case 3:
				boost::asio::write(socket, boost::asio::buffer("2"));
				break;
			case 4:
				boost::asio::write(socket, boost::asio::buffer("3"));
				break;
			default:
				std::cout << "invalid command" << std::endl;
				break;
			}

			size_t len = socket.read_some(boost::asio::buffer(buf), ignored_error);

			if (ignored_error == boost::asio::error::eof)
				break;
			else if (ignored_error)
				throw boost::system::system_error(ignored_error);
			if (len > 0)
			{
				std::cout.write(buf.data(), len);
				std::cout << std::endl;
			}
			else
			{
				std::cout << "no responce recieved" << std::endl;
			}
			std::cout << "Introduceti comanda" << std::endl;
			std::cout << "1. Daytime" << std::endl;
			std::cout << "2. Hello" << std::endl;
			std::cout << "3. Connected clients ip" << std::endl;
			std::cout << "0. Close client" << std::endl;
			std::cin >> option;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}