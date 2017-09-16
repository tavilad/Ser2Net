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

Server::Server(boost::asio::io_service& io_service, short port) : io_service_(io_service), acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
{
	start_accept();
}

void Server::start_accept()
{
	Session* new_session = new Session(io_service_);

	/*if (Serial::getInstance()->isOpen())
	{
	std::cout << "deschis deja" << std::endl;
	}
	else
	{
	std::cout << "am deschis acum" << std::endl;
	Serial::getInstance()->open("COM3");
	}*/
	acceptor_.async_accept(new_session->socket(),
		boost::bind(&Server::handle_accept, this, new_session,
		boost::asio::placeholders::error));
}

void Server::handle_accept(Session* new_session, const boost::system::error_code& error)
{
	if (!error)
	{
		new_session->start();
	}
	else
	{
		delete new_session;
	}

	start_accept();
}

Server::~Server()
{
}