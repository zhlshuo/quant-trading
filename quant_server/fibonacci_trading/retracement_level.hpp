#ifndef RETRACEMENT_LEVEL_HPP
#define RETRACEMENT_LEVEL_HPP

#include "mysql.hpp"
#include <string>
#include <utility>
#include <vector>
#include <ctime>
#include <unordered_map>
#include <algorithm>

using namespace std;

class RetracementLevel
{
private:
	unordered_map<string, vector<pair<string, double>>> symbol_closes;

	void get_symbol_closes()
	{
		time_t t = time(0);   // get time now
        	struct tm * now = localtime( & t );
	        long today =  (now->tm_year + 1900) * 10000 + (now->tm_mon + 1) * 100 + now->tm_mday;
		long one_year_ago = today - 10000;
		string query = "SELECT Symbol, Date, Close FROM Analytics.Quotes where Date >= '" + to_string(one_year_ago) + "'";
		MysqlManager *mysql_manager = MysqlManager::get_instance();
                sql::ResultSet* res = mysql_manager->executeQuery(query);

                while(res->next())
                {
                        symbol_closes[res->getString("Symbol")].push_back(make_pair(res->getString("Date"), res->getDouble("Close")));
                }
	}

	int find_peak(vector<pair<string, double>> time_series)
	{
		auto max_it = max_element(time_series.begin(), time_series.end(), 
						[](pair<string, double> elem1, pair<string, double> elem2){ // comparator for date and close pair
							return elem1.second < elem2.second;
						});

		return distance(time_series.begin(), max_it);
	}
public:
	RetracementLevel()
	{
	}

	void init()
	{
		get_symbol_closes();
	}

	unordered_map<string, vector<pair<string, double>>>& test_data()
	{
		return symbol_closes;
	}

	int test_find_peak(string symbol)
	{
		return find_peak(symbol_closes[symbol]);
	}
};

#endif
