object = get_quote
cc = g++
source = main.cpp
option = -pthread -std=c++11
cflag = -I/usr/local/include/quote -L/usr/lib -lmysqlcppconn -lquote -lcurl 

all: get_quote 

get_quote: $(source)
	$(cc) $(option) $(source) $(cflag) -o $(object) 
