# Shimon Cohen 315383133
# Nadav Spitzer 302228275

a.out: compileAll
	g++ *.o -lpthread -o ../../exe/Server.out
	rm -f *.o

compileAll: *.h *.cpp
	g++ -c *.cpp

run:
	./Server.out
