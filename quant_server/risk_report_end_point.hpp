#ifndef RISK_REPORT_END_POINT
#define RISK_REPORT_END_POINT

#include "end_point.hpp"
#include "var.hpp"
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "risk_report.hpp"
#include "book.hpp"

using namespace std;

class RiskReportEndPoint: public EndPoint
{
private:
	vector<string> report_list;

	RiskReport* create_risk_report_instance(string report_name, string book_id)
	{
		if(report_name=="VarianceCovarianceVAR")
			return new VarianceCovarianceVAR(Book(book_id));
		else
			return NULL;
	}
public:
        RiskReportEndPoint(int port):EndPoint(port)
	{
		report_list.push_back("VarianceCovarianceVAR");
	}

        void on_open(server *s, websocketpp::connection_hdl hdl)
	{
		string report_list_as_json = "{\"risk_reports\":[";
                for(auto it = report_list.begin(); it != report_list.end(); ++it)
                        report_list_as_json += "\"" + *it + "\",";
                
		report_list_as_json.pop_back(); // get ride of the last comma 
                report_list_as_json += "]}";

		s->send(hdl, report_list_as_json, websocketpp::frame::opcode::text);
	}

        void on_message(server *s, websocketpp::connection_hdl hdl, server::message_ptr msg)
        {
		stringstream ss(msg->get_payload());
                string report_name, book_id;
		ss >> report_name >> book_id;

		unique_ptr<RiskReport> risk_report(create_risk_report_instance(report_name, book_id));

		string report_result = "{";
		if(risk_report)
		{
		        unordered_map<string, double> risk_vals = risk_report->get_risk_values();
        		for(auto it = risk_vals.begin(); it != risk_vals.end(); ++it)
                		report_result += "\"" + it->first + "\":\"" + to_string(it->second) + "\",";

			report_result.pop_back();
		}
		else
		{
			report_result += "\"error_msg\":\"" + report_name + " for " + book_id + " is not available\"";
		}
	
		report_result += "}";

                s->send(hdl, report_result, msg->get_opcode());
        }

};


#endif
