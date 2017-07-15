object = insert_option_chain
cc = g++
source = main.cpp
option = -pthread -std=c++11
makefile_dir = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
root_dir = $(patsubst %/,%,$(dir $(makefile_dir)))
cflag = -I$(root_dir)/include -I$(root_dir)/common -L$(root_dir)/lib -lmysqlcppconn -I/home/lishuo/Desktop/quant_studio/common

all: insert_option_chain

insert_option_chain: $(source)
	$(cc) $(option) $(source) $(cflag) -o $(object) 
