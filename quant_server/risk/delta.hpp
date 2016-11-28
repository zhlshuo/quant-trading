#include "book.hpp"
#include <unordered_map>
#include <vector>

using namespace std;

class Delta
{
private:
	Book book;
	vector<string> assets;
public:
	Delta(Book _book):book(_book)
	{
		assets = book.assets();
	}

	unordered_map<string, double> risk_value()
	{
		unordered_map<string, double> delta;
		for(auto it = assets.begin(); it != assets.end(); ++it)
                {
                        unordered_map<string, double> price_change_up, price_change_down;
			price_change_up[*it] = 1;
                        price_change_down[*it] = -1;
			
			delta[*it] = (book.price_on_asset_price_change(price_change_up) - book.price_on_asset_price_change(price_change_down))/2.0;
                }

		return delta;
	}
};
