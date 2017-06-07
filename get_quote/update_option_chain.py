#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Tue Jun  6 22:08:13 2017

@author: lishuo
"""
import MySQLdb
from pandas_datareader.data import Options
import datetime
import urllib2

from bs4 import BeautifulSoup

site = "http://en.wikipedia.org/wiki/List_of_S%26P_500_companies"

def scrape_list():
    hdr = {'User-Agent': 'Mozilla/5.0'}
    req = urllib2.Request(site, headers=hdr)
    page = urllib2.urlopen(req)
    soup = BeautifulSoup(page)

    table = soup.find('table', {'class': 'wikitable sortable'})
    tickers = []
    for row in table.findAll('tr'):
        col = row.findAll('td')
        if len(col) > 0:
            ticker = str(col[0].string.strip())
            tickers.append(ticker)
        
    return tickers

def update_option_chain():
    
    conn = MySQLdb.connect(host= "127.0.0.1",
                      user="root",
                      passwd="zhuang123",
                      db="Analytics")
    x = conn.cursor()
    
    columns = ['ask', 'bid', 'change', 'contractSize', 'contractSymbol', 'currency', 'expiration', 
               'impliedVolatility', 'inTheMoney', 'lastPrice', 'lastTradeDate', 'openInterest', 'percentChange', 
               'strike', 'volume']
    
    dt_columns = ['expiration', 'lastTradeDate']
    try:
        for ticker in scrape_list():
            option = Options(ticker, 'yahoo')
            
            option_chain = option.get_all_data()
            stmt = 'INSERT INTO OptionQuotes VALUES (' + ','.join(['%s']*len(columns)) + ')'
            for option in option_chain['JSON'].tolist():
                print(option)
                values = [option[column] if column not in dt_columns else datetime.datetime.fromtimestamp(option[column]).strftime('%Y-%m-%d %H:%M:%S') for column in columns]
                values.append(datetime.datetime.now.strftime('%Y-%m-%d %H:%M:%S'))
                x.execute(stmt, tuple(values))
        conn.commit()
    except Exception as e:
        conn.rollback()
        raise(e)
    
    conn.close()
    
update_option_chain()