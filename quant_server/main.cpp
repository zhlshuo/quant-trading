#include <iostream>
//#include <mlpack/core.hpp>
#include "init_end_point.hpp"
#include "booking_end_point.hpp"
#include "risk_report_end_point.hpp"
#include "var.hpp"
#include "book.hpp"
#include <memory>
#include <unordered_map>
#include <string>

using namespace std;
//using namespace mlpack;

int main()
{
	cout << "connect to init_server" << endl;
	InitEndPoint init_end_point(9002);
	init_end_point.run();

	cout << "connect to booking_server" << endl;
	BookingEndPoint booking_end_point(9003);
	booking_end_point.run();

	cout << "connect to risk_report_server" << endl;
        RiskReportEndPoint risk_report_end_point(9004);
        risk_report_end_point.run();
	
	while(true){};
	cout << "finish" << endl;
	return 0;
}
