#define _CRT_SECURE_NO_WARNINGS
#include "Session.h"
#include "Serial.h"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>

using boost::asio::ip::tcp;

std::vector<Session*> participants;

std::string make_daytime_string()
{
	using namespace std;
	time_t now = time(0);
	return ctime(&now);
}

enum Commands
{
	DAYTIME,
	HELLO,
	LIST,
	OPEN_SERIAL_PORT,
	WRITE_TO_SERIAL
};

Session::Session(boost::asio::io_service& io_service) : socket_(io_service), io_service_(io_service)
{
	//serial = new Serial(io_service);
	//serial->open("COM4");
}

tcp::socket& Session::socket()
{
	return socket_;
}

void Session::start()
{
	std::cout << "Client connected. IP:  " << socket_.remote_endpoint().address().to_string() << std::endl;

	this->ip = socket_.remote_endpoint().address().to_string();

	participants.push_back(this);
	std::cout << participants.size() << std::endl;

	socket_.async_read_some(boost::asio::buffer(data_, 1024),
		boost::bind(&Session::handle_read, this,
		boost::asio::placeholders::error));
}

void Session::menu()
{
	std::cout << "am intrat pe meniu" << std::endl;

	int cmd = data_[0] - '0';
	//std::string cmd(data_);
	//std::cout << cmd.c_str() << std::endl;

	switch (cmd)
	{
	case Commands::DAYTIME:
	{
		std::cout << "day" << std::endl;

		text = make_daytime_string();

		boost::asio::async_write(socket_,
			boost::asio::buffer(text),
			boost::bind(&Session::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		break;
	}
	case Commands::HELLO:
	{
		text = "hi";

		std::cout << "hi" << std::endl;

		boost::asio::async_write(socket_,
			boost::asio::buffer(text),
			boost::bind(&Session::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

		break;
	}
	case Commands::LIST:
	{
		int nr = participants.size();
		if (participants.size() > 0)
		{
			for (Session* session : participants)
			{
				boost::asio::async_write(socket_,
					boost::asio::buffer(session->ip),
					boost::bind(&Session::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
			}
		}
		else
		{
			text = "empty list";
			boost::asio::async_write(socket_,
				boost::asio::buffer(text),
				boost::bind(&Session::handle_write, this,
				boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		}
		break;
	}
	case Commands::OPEN_SERIAL_PORT:
	{
		if (!Serial::getInstance(io_service_)->isOpen())
		{
			Serial::getInstance(io_service_)->open("COM4");
			boost::asio::async_write(socket_,
				boost::asio::buffer("Port serial deschis"),
				boost::bind(&Session::handle_write, this,
				boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			boost::asio::async_write(socket_,
				boost::asio::buffer("Port serial este deja deschis"),
				boost::bind(&Session::handle_write, this,
				boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		}
	}
	case Commands::WRITE_TO_SERIAL:
	{
		std::cout << "write serial" << std::endl;

		Serial::getInstance(io_service_)->write();
		boost::asio::async_write(socket_,
			boost::asio::buffer("am scris"),
			boost::bind(&Session::handle_write, this,
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		break;
	}
	}

	std::cout << "am iesit din meniu" << std::endl;

	//using memset in order to reset the data_ array
	memset(data_, 0, sizeof data_);
}

void Session::handle_write(const boost::system::error_code& error, size_t len)
{
	if (!error)
	{
		std::cout << "am trimis" << std::endl;
	}
	else
	{
		std::cout << error.message();

		delete this;
	}
}

void Session::handle_read(const boost::system::error_code& error)
{
	if (!error)
	{
		socket_.async_read_some(boost::asio::buffer(data_),
			boost::bind(&Session::handle_read, this,
			boost::asio::placeholders::error));

		menu();
	}
	else
		if ((boost::asio::error::eof == error) ||
			(boost::asio::error::connection_reset == error))
		{
			std::cout << "client disconnected" << std::endl;
			participants.pop_back();

			delete this;
		}
}

Session::~Session()
{
}