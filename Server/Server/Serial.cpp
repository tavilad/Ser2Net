#include "Serial.h"
#include <iostream>

//static Serial *instance = 0;
//static boost::asio::io_service ioservice;

Serial::Serial(boost::asio::io_service& io_service) :sPort(io_service)
{
}

/*Serial* Serial::getInstance()
{
if (!instance)
instance = new Serial(ioservice);
return instance;
}*/

void Serial::open(const std::string& portname)
//int baudrate,
//int charactersize,
//boost::asio::serial_port_base::parity::type parity,
//boost::asio::serial_port_base::stop_bits::type stopbits,
//boost::asio::serial_port_base::flow_control::type flowcontrol)
{
	sPort.open(portname);
	//sPort.set_option(boost::asio::serial_port_base::baud_rate(baudrate));
	//sPort.set_option(boost::asio::serial_port_base::character_size(charactersize));
	//sPort.set_option(boost::asio::serial_port_base::parity(parity));
	//sPort.set_option(boost::asio::serial_port_base::stop_bits(stopbits));
	//sPort.set_option(boost::asio::serial_port_base::flow_control(flowcontrol));
}

void Serial::write()
{
	sPort.async_write_some(boost::asio::buffer("test"),
		boost::bind(&Serial::handle_write,
		this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void Serial::handle_write(const boost::system::error_code& error, size_t len)
{
	if (!error)
	{
		std::cout << "am trimis pe serial " << len << std::endl;
	}
	else
	{
		std::cout << error.message() << std::endl;
		delete this;
	}
}

void Serial::read()
{
	sPort.async_read_some(boost::asio::buffer(data_, 512),
		boost::bind(&Serial::handle_read,
		this,
		boost::asio::placeholders::error));
}

void Serial::handle_read(const boost::system::error_code& error)
{
	if (!error)
	{
		std::cout << "am primit" << std::endl;
	}
	else
	{
		std::cout << error.message() << std::endl;
		delete this;
	}
}

bool Serial::isOpen()
{
	return sPort.is_open();
}

void Serial::close()
{
	sPort.close();
}

Serial::~Serial()
{
}