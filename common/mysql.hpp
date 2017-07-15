#ifndef MYSQL_H
#define MYSQL_H

#include <iostream>
#include <string>
#include <vector>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "configuration.hpp"

using namespace std;

class MysqlManager
{
private:
	const string mysql_server;
	const string mysql_user;
	const string mysql_pwd;
	const string database;
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::PreparedStatement *pstmt;
	static MysqlManager *instance;
	static Configuration* config;

public:
	// keep the constructor public to enable multiple threads update one table
	MysqlManager():mysql_server(config->server),mysql_user(config->db_username),mysql_pwd(config->db_password),database(config->db_name),con(NULL){
                driver = sql::mysql::get_driver_instance();
                con = driver->connect(mysql_server, mysql_user, mysql_pwd);
                con->setSchema(database);
        }

public:
	static MysqlManager* get_instance()
	{
		if(!instance)
			instance = new MysqlManager();

		return instance;
	}

	int executeUpdate(string query, vector<vector<string>> values)
	{
		int row_affected = 0;
		
		pstmt = con->prepareStatement(query);
		
		for(int value_index = 0; value_index < values.size(); ++value_index)
		{
			for(int field_index = 0; field_index < values[value_index].size(); ++field_index)
			{
				pstmt->setString(field_index+1, values[value_index][field_index]);
			}
			
			try
	                {
				row_affected += pstmt->executeUpdate();
			}catch(sql::SQLException &e)
                	{
        	                cout << "fail to update:" << e.what() << endl;
	                }

		}
		
		delete pstmt;
		return row_affected;
	}

	sql::ResultSet* executeQuery(string query)
	{
		stmt = con->createStatement();
		sql::ResultSet* rs = stmt->executeQuery(query);
		delete stmt;
		return rs;
	}

	~MysqlManager()
	{
		delete con;
	}
};

MysqlManager *MysqlManager::instance = NULL;
Configuration *MysqlManager::config = Configuration::get_instance();

#endif
