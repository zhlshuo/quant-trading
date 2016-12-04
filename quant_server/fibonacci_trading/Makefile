object = quant_server
cc = g++
source = main.cpp
option = -pthread -std=c++11
cflag = -lmlpack -larmadillo -lboost_serialization -lboost_program_options -lboost_system -lmysqlcppconn -I/home/lishuo/Desktop/quant_studio/quant_server -I/home/lishuo/Desktop/quant_studio/quant_server/position -I/home/lishuo/Desktop/quant_studio/quant_server/risk -I/usr/local/include/ql -I/home/lishuo/Desktop/quant_studio/common -lQuantLib

all: quant_server

quant_server: $(source)
	$(cc) $(option) $(source) $(cflag) -o $(object) 
