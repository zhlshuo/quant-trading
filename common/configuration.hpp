#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>

using namespace std;

class Configuration {

private:
	static bool dev_env;
	static Configuration *instance;

        Configuration()
	{
		debug = true;
		if(dev_env)
		{
			server = "tcp://127.0.0.1:3306";
			db_username = "root";
			db_password = "zhuang123";
			db_name = "Analytics";
		}
		else
		{
			server = "tcp://107.180.44.139:3306";
                        db_username = "zhuanglishuo";
                        db_password = "zhuanglishuo123";
                        db_name = "QuantStudio";
		}
        }

public:
	string server;
	string db_username;
	string db_password;
	string db_name;
	bool debug; // debug mode or not

        static Configuration* get_instance()
        {
                if(!instance)
                        instance = new Configuration();

                return instance;
        }

        ~Configuration()
        {
        }
};

bool Configuration::dev_env = true;
Configuration *Configuration::instance = NULL;


#endif
