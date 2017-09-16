#define _CRT_SECURE_NO_WARNINGS
#include "Session.h"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <string>
#include <set>

using boost::asio::ip::tcp;

class Server
{
public:
	Server(boost::asio::io_service& io_service, short port);
	void start_accept();
	void handle_accept(Session* new_session, const boost::system::error_code& error);
	~Server();

private:
	boost::asio::io_service& io_service_;
	tcp::acceptor acceptor_;
};
