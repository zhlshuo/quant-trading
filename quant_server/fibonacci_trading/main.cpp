#include "retracement_level.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int main()
{
	RetracementLevel rl;
	rl.init();
	unordered_map<string, vector<pair<string, double>>> symbol_closes = rl.test_data();

	double max_close = -1;
	for(auto it = symbol_closes["GOOG"].begin(); it != symbol_closes["GOOG"].end(); ++it)
	{
		if(max_close < it->second)
			max_close = it->second;
		cout << it->first << ": " << it->second << endl;
	}
	cout << "finish getting data" << endl;

	cout << "max:" << max_close << endl;
	cout << symbol_closes["GOOG"][rl.test_find_peak("GOOG")].second << endl;
	return 0;
}
