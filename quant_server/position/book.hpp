#ifndef BOOK_H
#define BOOK_H

#include "mysql.hpp"
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

struct AssetEconomics
{
	double quantity;
	double price;

	AssetEconomics()
	{
	}

	AssetEconomics(AssetEconomics& ae)
	{
		quantity = ae.quantity;
		price = ae.price;
	}
	
	AssetEconomics(const AssetEconomics& ae)
	{
		quantity = ae.quantity;
		price = ae.price;
	}
};

class Book
{
private:
	unordered_map<string, double> deals; // ticker and quantity map
	unordered_map<string, double> ticker_price; // ticker and price map
	unordered_map<string, AssetEconomics> asset_economics; // ticker, quant/price map
public:
	Book(){}

	Book(Book& book)
	{
		deals = book.deals;
		ticker_price = book.ticker_price;
		asset_economics = book.asset_economics;
	}	

	Book(const Book& book)
	{
		deals = book.deals;
		ticker_price = book.ticker_price;
		asset_economics = book.asset_economics;
	}

	Book(string ID, bool trading_book=true)
	{
		MysqlManager *mysql_manager = MysqlManager::get_instance();

		// get deals of book
		string deal_query = "";

		if(trading_book)
			deal_query = "SELECT * FROM Deal where Book1_ID=\"" + ID + "\"";
		else
			deal_query = "SELECT * FROM Deal where Book2_ID=\"" + ID + "\"";

        	sql::ResultSet* res = mysql_manager->executeQuery(deal_query);

        	while(res->next())
        	{
                	string ticker = res->getString("Ticker");
			double quantity = stod(res->getString("Quantity"));
			if(deals.find(ticker) == deals.end())
				deals[ticker] = quantity;
			else
				deals[ticker] += quantity;
        	}

		// get close price of tickers
		string tickers = "(";
                for(auto it = deals.begin(); it != deals.end(); ++it)
                	tickers += "\"" + it->first + "\",";

		tickers.pop_back();
                tickers += ")";

                string quote_query = "SELECT * FROM Quotes where symbol in " + tickers +  " and Date=(select max(Date) from Quotes)";

                res = mysql_manager->executeQuery(quote_query);

                while(res->next())
                {
			string ticker = res->getString("Symbol");
                        double price = stod(res->getString("Close"));

                        ticker_price[ticker] = price;
                }

		// store asset quantity and price
		for(auto it = deals.begin(); it != deals.end(); ++it)
		{
			AssetEconomics ae;
			ae.quantity = it->second;
			ae.price = ticker_price[it->first];
			asset_economics[it->first] = ae;
		}
	}

	double price(string date = "latest_date")
	{
		// book can price on any specific date
		if(date != "latest_date")
		{
			string tickers = "(";
                	for(auto it = deals.begin(); it != deals.end(); ++it)
                	        tickers += "\"" + it->first + "\",";

        	        tickers.pop_back();
	                tickers += ")";

			MysqlManager *mysql_manager = MysqlManager::get_instance();
			string quote_query = "SELECT * FROM Quotes where symbol in " + tickers +  " and Date=Date(\"" + date + "\")";

	                sql::ResultSet* res = mysql_manager->executeQuery(quote_query);

        	        while(res->next())
                	{
                        	string ticker = res->getString("Symbol");
	                        double price = stod(res->getString("Close"));

        	                ticker_price[ticker] = price;
                	}

		}

		double book_price = 0;
		for(auto it = deals.begin(); it != deals.end(); ++it)
		{
			book_price += it->second * ticker_price[it->first];
		}

		return book_price;
	}

	double price_on_asset_price_change(unordered_map<string, double> price_changes)
	{
		unordered_map<string, double> ticker_price_copy = ticker_price;
		for(auto it = price_changes.begin(); it != price_changes.end(); ++it)
		{
			ticker_price[it->first] += it->second;
		}

		double book_price = this->price();
		ticker_price = ticker_price_copy;

		return book_price;
	}

	vector<string> assets()
	{
		vector<string> asset;
		for(auto it = ticker_price.begin(); it != ticker_price.end(); ++it)
			asset.push_back(it->first);

		return asset;
	}

	unordered_map<string, AssetEconomics> assets_economics()
	{
		return asset_economics;
	}
};

#endif
