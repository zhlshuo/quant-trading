object = risk_report
cc = g++
source = main.cpp
option = -pthread -std=c++11
cflag = -lmysqlcppconn -I/home/lishuo/Desktop/quant_studio/quant_server -I/home/lishuo/Desktop/quant_studio/quant_server/position -I/home/lishuo/Desktop/quant_studio/quant_server/risk -I/home/lishuo/Desktop/quant_studio/common -I/usr/local/include/ql -lQuantLib

all: risk_report

risk_report: $(source)
	$(cc) $(option) $(source) $(cflag) -o $(object) 
