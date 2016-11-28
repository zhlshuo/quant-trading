#ifndef HISTORICAL_VAR_HPP
#define HISTORICAL_VAR_HPP

#include "risk_report.hpp"
#include <unordered_map>
#include "book.hpp"

using namespace std;

class HistoricalVAR
{
private:
	double confidence_level;
	Book _book;
public:
	HistoricalVAR(Book book, cl = 0.99):_book(book)
	{
		confidence_level = cl;
	}

	unordered_map<string, double> get_risk_values()
	{
		unordered_map<string, double> var;
		var["confidence_level"] = confidence_level;

		return var;
	}
};

#endif
