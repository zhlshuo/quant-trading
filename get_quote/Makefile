object = get_quote
cc = g++
source = main.cpp
option = -pthread -std=c++11
cflag = -I/usr/include -I/usr/local/include/quote -I/home/lishuo/Desktop/quant_studio/common -L/../lib -lquote -lcurl -lmysqlcppconn

all: get_quote 

get_quote: $(source)
	$(cc) $(option) $(source) $(cflag) -o $(object) 
