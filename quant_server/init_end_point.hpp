#ifndef INIT_END_POINT_HPP
#define INIT_END_POINT_HPP

#include "end_point.hpp"
#include "mysql.hpp"
#include <string>
#include <sstream>

using namespace std;

class InitEndPoint: public EndPoint
{
private:
	string deals;
	string tickers;
	string books;
	string init_msg;

	string get_deals_as_json(string book_id="1")
        {
                MysqlManager *mysql_manager = MysqlManager::get_instance();
                sql::ResultSet* res = mysql_manager->executeQuery("select * from Deal where Book1_ID = " + book_id);
		
		string deals = "{\"book_id\":\"" + book_id + "\",\"deals\":[";
                while(res->next())
                {
                        deals += "{\"ticker\":\"" + (res->getString("Ticker")) + "\",";
                        deals += "\"quantity\":\"" + (res->getString("Quantity")) + "\",";
                        deals += "\"date\":\"" + (res->getString("Date")) + "\"},";
                }

                deals.pop_back(); // get ride of the last comma 
                deals += "]}";

                return deals;
        }	

	string get_tickers_as_json()
	{
	        tickers = "{\"tickers\":[";

        	MysqlManager *mysql_manager = MysqlManager::get_instance();
	        sql::ResultSet* res = mysql_manager->executeQuery("select distinct(Symbol) from Quotes");

        	while(res->next())
	        {
        	        tickers += "\"" + (res->getString("Symbol")) + "\",";
	        }

        	tickers.pop_back(); // get ride of the last comma
	        tickers += "]}";

        	return tickers;
	}

	string get_quotes_as_json(string ticker)
	{
        	string query = "select Date, Open, High, Low, Close from Quotes where Symbol='" + ticker + "' order by Date";
	        cout << query << endl;
        	MysqlManager *mysql_manager = MysqlManager::get_instance();
	        sql::ResultSet* res = mysql_manager->executeQuery(query);

        	string quotes = "{\"ticker\":\"" + ticker + "\", \"quotes\":[";
	        while(res->next())
        	{
                	quotes += "{\"date\":\"" + (res->getString("Date")) + "\",";
	                quotes += "\"open\":\"" + (res->getString("Open")) + "\",";
        	        quotes += "\"high\":\"" + (res->getString("High")) + "\",";
                	quotes += "\"low\":\"" + (res->getString("Low")) + "\",";
	                quotes += "\"close\":\"" + (res->getString("Close")) + "\"},";
        	}

        	quotes.pop_back(); // get ride of the last comma 
	        quotes += "]}";

        	return quotes;
	}

	string get_books_as_json()
	{
        	MysqlManager *mysql_manager = MysqlManager::get_instance();
	        books = "{\"books\":{\"trading_book\":[";

        	sql::ResultSet* res = mysql_manager->executeQuery("select * from Trading_Book");

	        while(res->next())
        	{
                	books += "{\"ID\":\"" + (res->getString("ID")) + "\",";
	                books += "\"Name\":\"" + (res->getString("Name")) + "\",";
        	        books += "\"ParentID\":\"" + (res->getString("ParentID")) + "\"},";
	        }

        	books.pop_back(); // get ride of the last comma
	        books += "], \"customer_book\":[";

        	res = mysql_manager->executeQuery("select * from Customer_Book");

	        while(res->next())
        	{
                	books += "{\"ID\":\"" + (res->getString("ID")) + "\",";
	                books += "\"Name\":\"" + (res->getString("Name")) + "\",";
        	        books += "\"ParentID\":\"" + (res->getString("ParentID")) + "\"},";
	        }

        	books.pop_back(); // get ride of the last comma
	        books += "]}}";

        	return books;
	}

	string merge_json(string json1, string json2)
	{
        	// get rid of the right parantheses and append comma of the first json
	        json1.pop_back();
        	json1 += ",";

	        // get rid of the left paratheses
        	json2.erase(0, 1);

	        return json1 + json2;
	}

public:
	InitEndPoint(int port):EndPoint(port)
	{
		deals = "";
		tickers = "";
		books = "";
		init_msg = "";
	}

	void on_open(server *s, websocketpp::connection_hdl hdl)
	{
		if(deals == "")
			deals = get_deals_as_json();

		if(tickers == "") 
                	tickers = get_tickers_as_json();

	        if(books == "") 
        	        books = get_books_as_json();

	        if(init_msg == "") 
		{
			init_msg = merge_json(deals, tickers);
        	        init_msg = merge_json(init_msg, books);
		}

	        s->send(hdl, init_msg, websocketpp::frame::opcode::text);
	}

	void on_message(server *s, websocketpp::connection_hdl hdl, server::message_ptr msg)
	{
		stringstream ss(msg->get_payload());
                string msg_type, msg_val;
                ss >> msg_type >> msg_val;

		string response = "";

		if(msg_type=="ticker_for_chart")
		{
			response = get_quotes_as_json(msg_val); // msg_val is a ticker
		}
		else if(msg_type=="book_id_for_deals")
		{
			response = get_deals_as_json(msg_val); // msg_val is a book id
		}

	        s->send(hdl, response, msg->get_opcode());
	}
};

#endif
