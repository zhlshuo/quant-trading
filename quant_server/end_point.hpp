#ifndef END_POINT_HPP
#define END_POINT_HPP

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <thread>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using namespace std;

class EndPoint
{
private:
	int port;
	server _server;
public:
	EndPoint(int port)
	{
		this->port = port;
	}

	virtual void on_open(server *s, websocketpp::connection_hdl hdl) = 0;
	virtual void on_message(server *s, websocketpp::connection_hdl hdl, server::message_ptr msg) = 0;

	void run()
	{
		_server.init_asio();
		_server.set_open_handler(bind(&EndPoint::on_open, this, &_server, ::_1));
		_server.set_message_handler(bind(&EndPoint::on_message, this, &_server, ::_1, ::_2));
		_server.listen(port);
		_server.start_accept();
		thread(&server::run, &_server).detach();
	}
};

#endif
