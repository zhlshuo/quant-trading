#include <iostream>
#include "delta.hpp"
#include <unordered_map>
#include <string>
#include "book.hpp"
#include "var.hpp"
#include "risk_report.hpp"
#include "memory"

using namespace std;

int main()
{
	/*cout.precision(17);
	Book book("1");
	auto asset = book.assets_economics();
	for(auto it = asset.begin(); it != asset.end(); ++it)
		cout << it->first << ":" << it->second.quantity << ", " << it->second.price << endl;
	Delta delta_report(book);

	unordered_map<string, double> delta = delta_report.risk_value();

	for(auto it = delta.begin(); it != delta.end(); ++it)
		cout << it->first << " " << it->second << endl;

	cout << "finish" << endl;*/
	
	unique_ptr<RiskReport> var(new VarianceCovarianceVAR(Book("1")));
	unordered_map<string, double> risk_vals = var->get_risk_values();
	for(auto it = risk_vals.begin(); it != risk_vals.end(); ++it)
		cout << it->first << ":" << it->second << endl;
	

	return 0;
}
