/* This file aims to calculate value at risk in a different methodology.
* 1. The VarCovarVaR adopt the Delta Normal VaR methodology. Until writting this comment, the only risk factor
* considered is price, no derivatives are used. Hence the delta = ds/ds = 1.0, so no delta is used.
* 2. HistoricalVaR
*/

#ifndef VAR_H
#define VAR_H

#include "book.hpp"
#include <unordered_map>
#include "mysql.hpp"
#include <string>
#include <math/matrix.hpp>
#include <numeric>
#include <iostream>
//#include "debug.hpp"
#include "risk_report.hpp"
#include "configuration.hpp"

using namespace std;
using namespace QuantLib;

class VarianceCovarianceVAR: public RiskReport
{
private:
	bool debug;
	Book book;
	double confidence_level;
	unordered_map<string, AssetEconomics> assets_economics;
	int days_look_back;
	unordered_map<string, vector<double>> assets_quotes;
	unordered_map<string, vector<double>> assets_returns;
	void print_matrix(Matrix m)
	{
		for(int row = 0; row < m.rows(); ++row)
		{
			for(int col = 0; col < m.columns(); ++col)
			{
				cout << m[row][col] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	// weight * covariance * transpose(weight)
	double std_dev()
	{
		Matrix weights(1, assets_economics.size());
		Matrix risk_factor_matrix(assets_economics.size(), days_look_back);
		
		int asset_index = 0;
		double notional = book.price();

		for(auto it = assets_economics.begin(); it != assets_economics.end(); ++it, ++asset_index)
		{
			weights[0][asset_index] = (it->second.quantity * it->second.price)/notional;
			vector<double> asset_returns = assets_returns[it->first];
			double sum = accumulate(asset_returns.begin(), asset_returns.end(), 0.0);
			double mean = sum/asset_returns.size();

			for(int i = 0; i < asset_returns.size(); ++i)
			{
				risk_factor_matrix[asset_index][i] = asset_returns[i] - mean;
			}
		}
		
		Matrix covariance_matrix(risk_factor_matrix * transpose(risk_factor_matrix));

		Matrix res_matrix =  weights * covariance_matrix * transpose(weights);
		double std_dev = res_matrix[0][0];
		
		if(debug)
		{
			cout << "book price: " << book.price() << endl << endl;
			cout << "weight:" << endl;
			print_matrix(weights);
			cout << "risk factor:" << endl;
			print_matrix(risk_factor_matrix);
			cout << "covariance matrix:" << endl;
			print_matrix(covariance_matrix);
			cout << "result:" << endl;
			print_matrix(res_matrix);
			cout << "sdv: " << std_dev << endl;
		}

		return std_dev;
	}

	void get_assets_returns()
	{
		for(auto asset_quotes_it = assets_quotes.begin(); asset_quotes_it != assets_quotes.end(); ++asset_quotes_it)
		{
			vector<double> asset_quotes = asset_quotes_it->second;
			for(int i = 0;  i < asset_quotes.size()-1; ++i)
			{
				assets_returns[asset_quotes_it->first].push_back((asset_quotes[i+1] - asset_quotes[i])/asset_quotes[i]);
			}
		}
	}

	// get Z. For confidence level 0.99, Z = 2.33
	double get_z_score()
	{
		return 2.33;
	}

	void get_quotes()
	{
		MysqlManager *mysql_manager = MysqlManager::get_instance();

		string tickers = "(";
                for(auto it = assets_economics.begin(); it != assets_economics.end(); ++it)
                        tickers += "\"" + it->first + "\",";

                tickers.pop_back();
                tickers += ")";
		
		int record_num = days_look_back * assets_economics.size();
		string quote_query = "SELECT * FROM Quotes where symbol in " + tickers +  " order by Date desc limit " + to_string(record_num);

                sql::ResultSet* res = mysql_manager->executeQuery(quote_query);

                while(res->next())
                {
                        string ticker = res->getString("Symbol");
                        double price = stod(res->getString("Close"));

                        assets_quotes[ticker].push_back(price);
		}

		get_assets_returns();
	}

public:
	VarianceCovarianceVAR(Book _book, double cl=0.99, int days_look_back=252):book(_book)
	{
		confidence_level = cl;
		assets_economics = book.assets_economics();
		this->days_look_back = days_look_back;
		get_quotes();
		debug = Configuration::get_instance()->debug;
	}

	unordered_map<string, double> get_risk_values()
	{
		unordered_map<string, double> stat;
		stat["book_price"] = book.price();
		stat["var"] = stat["book_price"] * std_dev() * get_z_score(); // notional * sdv * z_score
		stat["confidence_level"] = confidence_level;

		return stat;
	}
};

#endif
