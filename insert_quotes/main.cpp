#include <fstream>
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
#include <thread>
#include <memory>
using namespace std;

int nThread = 120;
string base_dir = "/home/lishuo/Desktop/OptionChain_all/";

vector<string> get_tickers()
{
	vector<string> tickers;
	
	MysqlManager *mysql_manager = MysqlManager::get_instance();
	sql::ResultSet* res = mysql_manager->executeQuery("select ticker from OptionTicker");
	
	while(res->next())
	{
		tickers.push_back(res->getString("ticker"));
	}

	return tickers;
}

int insert_option_chain(string ticker)
{
	vector<vector<string>> insert_values;
	string query = "insert into HistoricalOptionChain (ticker, type, underlyingPrice, mid, pricingDate, strike, expiration) values(?,?,?,?,?,?,?)";

        try{
		string file_name = base_dir + ticker;

		ifstream file(file_name);
		if(file.is_open())
		{
			string line;
			vector<string> vals;
			while(getline(file, line))
			{
				vector<string> insert_val ={ticker};
				boost::split(vals,line,boost::is_any_of("/"));
				insert_val.insert(insert_val.end(), vals.begin() + 1, vals.end());
				insert_values.push_back(insert_val);
			}
		}

        }catch(const std::exception &exc){
                cout << "failed to get quotes for ticker " << ticker << " because:" << exc.what() << endl;
        }

	vector<thread> threads;
	int blockSize = insert_values.size()/nThread;
	for(int i = 0; i < nThread - 1; ++i)
	{
		vector<vector<string>> block(insert_values.begin() + i*blockSize, insert_values.begin() + (i+1)*blockSize);
		threads.push_back(thread(&MysqlManager::executeUpdate, unique_ptr<MysqlManager>(new MysqlManager()), query, block));
	}
	
	vector<vector<string>> last_block(insert_values.begin()+(nThread - 1)*blockSize, insert_values.end());
	threads.push_back(thread(&MysqlManager::executeUpdate, unique_ptr<MysqlManager>(new MysqlManager()), query, last_block));

	for(auto it = threads.begin(); it != threads.end(); ++it)
		it->join();

	cout << "finish insert for " << ticker << endl;

	return 0;
}

int main(int argc, const char * argv[]) {
	vector<string> tickers = get_tickers();
	//vector<string> tickers_subset(tickers.begin(), tickers.begin()+50);
	time_t st = time(0);   // get time now
	
	for(auto it = tickers.begin(); it != tickers.end(); ++it)
		insert_option_chain(*it);
	
	time_t ed = time(0);

	double seconds = difftime(ed, st);
	cout << seconds << " seconds" << endl;
	
	cout << "finish" << endl;
	return 0;
}
