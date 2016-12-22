object = fibonacci_trading
cc = g++
source = main.cpp
option = -pthread -std=c++11
cflag = -lmysqlcppconn -I/home/lishuo/Desktop/quant_studio/common -I/usr/include/python2.7 -lpython2.7

all: fibonacci_trading

fibonacci_trading: $(source)
	$(cc) $(option) $(source) $(cflag) -o $(object) 
