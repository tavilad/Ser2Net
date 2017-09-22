#pragma once
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

class Serial
{
public:
	static Serial* getInstance(boost::asio::io_service& io_service);
	void open(const std::string& portname);
	void close();
	bool isOpen();
	//int baudrate,
	//int charactersize,
	//boost::asio::serial_port_base::parity::type parity,
	//boost::asio::serial_port_base::stop_bits::type stopbits,
	//boost::asio::serial_port_base::flow_control::type flowcontrol);
	void write();
	std::string read();
	void handle_write(const boost::system::error_code& error, size_t len);
	void handle_read(const boost::system::error_code& error);
	~Serial();
private:
	Serial(boost::asio::io_service& io_service);
	boost::asio::serial_port sPort;
	char data_[50];
};