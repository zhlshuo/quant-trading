#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Tue Jun  6 18:40:19 2017

@author: lishuo
"""

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

#print(scrape_list())