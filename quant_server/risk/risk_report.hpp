#ifndef RISK_REPORT_HPP
#define RISK_REPORT_HPP

#include <unordered_map>

using namespace std;

class RiskReport
{
public:
	virtual unordered_map<string, double> get_risk_values() = 0;

	virtual ~RiskReport(){}
};

#endif
