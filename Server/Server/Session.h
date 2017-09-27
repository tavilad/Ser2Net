#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "Serial.h"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <string>
#include <set>

using boost::asio::ip::tcp;

enum Commands;

class Session
{
public:
	Session(boost::asio::io_service& io_service);
	tcp::socket& socket();
	void start();
	void menu();
	void handle_write(const boost::system::error_code& error, size_t len);
	void handle_read(const boost::system::error_code& error);
	~Session();

private:
	tcp::socket socket_;
	char data_[50];
	std::string text;
	std::string commandd;
	std::string ip;
	boost::asio::io_service& io_service_;
};
