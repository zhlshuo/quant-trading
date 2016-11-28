#ifndef BOOKING_END_POINT_HPP
#define BOOKING_END_POINT_HPP

#include "end_point.hpp"
#include "mysql.hpp"
#include <string>
#include <sstream>

using namespace std;

class BookingEndPoint: public EndPoint
{
public:
	BookingEndPoint(int port):EndPoint(port){}

	void on_open(server *s, websocketpp::connection_hdl hdl){}

        void on_message(server *s, websocketpp::connection_hdl hdl, server::message_ptr msg)
        {
		stringstream ss(msg->get_payload());
	        string book1_id, book2_id, ticker, quantity, date;
        	ss >> book1_id >> book2_id >> ticker >> quantity >> date;
	        vector<vector<string>> insert_vals;

        	vector<string> insert_val;
	        insert_val.push_back(book1_id);
        	insert_val.push_back(book2_id);
	        insert_val.push_back(ticker);
        	insert_val.push_back(quantity);
	        insert_val.push_back(date);

        	insert_vals.push_back(insert_val);

	        MysqlManager *mysql_manager = MysqlManager::get_instance();
        	int row_affected = mysql_manager->executeUpdate("insert into Deal(Book1_ID, Book2_ID, Ticker, Quantity, Date) values(?, ?, ?, ?, ?) ON DUPLICATE KEY UPDATE Quantity=Quantity+"+quantity, insert_vals);
	        s->send(hdl, to_string(row_affected), msg->get_opcode());
        }

};

#endif
