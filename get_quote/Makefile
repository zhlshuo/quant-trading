object = get_quote
cc = g++
source = main.cpp
option = -pthread -std=c++11
makefile_dir = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
root_dir = $(patsubst %/,%,$(dir $(makefile_dir)))
cflag = -I$(root_dir)/include -I$(root_dir)/common -L$(root_dir)/lib -lquote -lcurl -lmysqlcppconn -lcurlpp -I/home/lishuo/Desktop/quant_studio/common -I/usr/include/python2.7 -lpython2.7

all: get_quote 

get_quote: $(source)
	$(cc) $(option) $(source) $(cflag) -o $(object) 
