#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Tue Jun  6 17:45:19 2017

@author: lishuo
"""

import MySQLdb
from pandas_datareader.data import Options
import datetime

def update_option_chain(ticker):
    conn = MySQLdb.connect(host= "127.0.0.1",
                      user="root",
                      passwd="zhuang123",
                      db="Analytics")
    x = conn.cursor()
    
    columns = ['ask', 'bid', 'change', 'contractSize', 'contractSymbol', 'currency', 'expiration', 'impliedVolatility', 
               'inTheMoney', 'lastPrice', 'lastTradeDate', 'openInterest', 'percentChange', 'strike', 'volume']
    
    dt_columns = ['expiration', 'lastTradeDate']
    try:
        aapl = Options(ticker, 'yahoo')
        
        option_chain = aapl.get_all_data()
        stmt = 'INSERT INTO OptionQuotes VALUES (' + ','.join(['%s']*15) + ')'
        for option in option_chain['JSON'].tolist():
            x.execute(stmt,tuple([option[column] if column not in dt_columns else datetime.datetime.fromtimestamp(option[column]).strftime('%Y-%m-%d %H:%M:%S') for column in columns]))
            conn.commit()
    except Exception as e:
        conn.rollback()
        print(e)
    
    conn.close()
    
#update_option_chain('GOOG')