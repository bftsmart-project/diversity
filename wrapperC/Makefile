INCL=-I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
CCFLAGS=-g -Wextra -Wall -Werror
CPFLAGS=-g -Wextra -Wall -Werror -std=c++0x

wrapper:
	g++ $(CPFLAGS) -fPIC -o wrapper.o $(INCL) $(LIBS) -c bftsmart-wrapper.cpp -ljvm 

test_client: wrapper
	gcc $(CCFLAGS) -o test_client.out $(INCL) $(LIBS) test_client.c wrapper.o -ljvm

test_server: wrapper
	gcc $(CCFLAGS) -o test_server.out $(INCL) $(LIBS) test_server.c wrapper.o -ljvm

tests: test_server test_client

lshared: wrapper
	g++ -shared -Wl,-soname,libbftsmr.so $(LIBS) -o libbftsmr.so wrapper.o -ljvm

all: wrapper lshared

.PHONY: clean

clean:
	rm -fv *.o *~ *.out



   


