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
               'strike', 'volume', 'pricingDate', 'underlying', 'underlyingPrice', 'type', 'mid']
    
    dt_columns = ['expiration', 'lastTradeDate']
    
    for ticker in scrape_list():
        try:
            option = Options(ticker, 'yahoo')
            
            option_chain = option.get_all_data()
            stmt = 'INSERT INTO OptionQuotes VALUES (' + ','.join(['%s']*len(columns)) + ')'
            for index, option in enumerate(option_chain['JSON'].tolist()):
                option['pricingDate']     = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                option['underlying']      = option_chain.iloc[index]['Underlying']
                option['underlyingPrice'] = option_chain.iloc[index]['Underlying_Price']
                option['type']            = 'call' if (option['underlyingPrice'] > option['strike'] and option['inTheMoney']) or (option['underlyingPrice'] < option['strike'] and not option['inTheMoney']) else 'put'
                option['mid'] = (option['ask'] + option['bid'])/2.
                values = [option[column] if column not in dt_columns else datetime.datetime.fromtimestamp(option[column]).strftime('%Y-%m-%d %H:%M:%S') for column in columns]
                print 'update', ticker, 'with values: ', values
                x.execute(stmt, tuple(values))
        except Exception as e:
            print 'failed to update', ticker, 'becasue:', str(e)
    conn.commit()
    
    
    conn.close()
    
update_option_chain()