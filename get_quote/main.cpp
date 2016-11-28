#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "quote.hpp"
#include "mysql.hpp"
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/iterator_range.hpp>
#include <ctime>

using namespace std;

vector<string> get_tickers()
{
	vector<string> tickers;
	
	MysqlManager *mysql_manager = MysqlManager::get_instance();
	sql::ResultSet* res = mysql_manager->executeQuery("select Symbol from Tickers");
	
	while(res->next())
	{
		tickers.push_back(res->getString("Symbol"));
	}

	return tickers;
}

int update_quotes(vector<string> tickers, long long st_date, long long ed_date)
{
	vector<vector<string>> insert_values;
	string query = "";

	for(int ticker_index = 0; ticker_index < tickers.size(); ++ticker_index)
        {
		int st_year  = st_date/10000;
                int st_month = (st_date%10000)/100;
                int st_day   = st_date%100;

                int ed_year  = ed_date/10000;
                int ed_month = (ed_date%10000)/100;
                int ed_day   = ed_date%100;

                try{
                	string response_string = quote::getHistoricalQuotesCsv(tickers[ticker_index],
                                                                               st_year, st_month, st_day,
                                                                               ed_year, ed_month, ed_day,
                                                                               quote::RangeType::daily);
			stringstream response(response_string);
				
			// construct the headers/columns of the table in database, replace the space in header with underscore
			string headers;
			getline(response, headers, '\n');
			replace(headers.begin(), headers.end(), ' ', '_');

			// construct the insert statement: prepare statement + values
			// prepare statment is only constructed at the first time 
			if(query=="")
			{
				vector<string> headers_list;
				boost::split(headers_list, headers, boost::is_any_of(","));
				vector<string> place_holders(headers_list.size(), "?");
                                query = "insert into Quotes (Symbol," + headers + ") values(?," + boost::algorithm::join(place_holders, ",") + ")";
			}

			string values;
			while(getline(response, values, '\n'))
			{
				// construct record consist of: symbol, date, open, high, low, close, volume, adjust close
				vector<string> insert_value;
				insert_value.push_back(tickers[ticker_index]);
				vector<string> vals;
				boost::split(vals, values, boost::is_any_of(","));
				insert_value.insert(insert_value.end(), vals.begin(), vals.end());

				// add the record to values inserted
				insert_values.push_back(insert_value);
			}
                }catch(...){
                        cout << "failed to get quotes for ticker " << tickers[ticker_index] << endl;
                }
        }

	MysqlManager *mysql_manager = MysqlManager::get_instance();
	return mysql_manager->executeUpdate(query, insert_values);
}

int main(int argc, const char * argv[]) {
	//time_t t = time(0);   // get time now
        //struct tm * now = localtime( & t );
        //long long start_date =  (now->tm_year + 1900) * 10000 + (now->tm_mon + 1) * 100 + now->tm_mday;
	//long long end_date =  (now->tm_year + 1900) * 10000 + (now->tm_mon + 1) * 100 + now->tm_mday;

	long long start_date = stoll(argv[1]);
        long long end_date   = stoll(argv[2]);
	
	//long long start_date = 20040101;
        //long long end_date   = 20141231;

        cout << start_date << " to " << end_date << endl;

	auto tickers = get_tickers();
 	update_quotes(tickers, start_date, end_date);

	// latest quote
	/*string petr4Quotes = quote::getLatestQuotesCsv("MSFT", {quote::QuoteType::symbol,
                                                                quote::QuoteType::name,
                                                                quote::QuoteType::lastTradePriceOnly,
                                                                quote::QuoteType::lastTradeDate,
                                                                quote::QuoteType::lastTradeTime});
	cout << petr4Quotes << endl;
	cout << tickers.size() << " " << tickers[0] << endl;*/
	
	cout << "finish" << endl;

	return 0;
}
