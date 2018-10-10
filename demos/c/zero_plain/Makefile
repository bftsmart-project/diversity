PASTAJAVA := $(shell ../../../scripts/findjava.sh)
INCL=-I../../../diversity/c/headers -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux 
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server

CCFLAGS=-g -Wextra -Wall $(INCL) 
CPFLAGS=-g -Wextra -Wall -fpermissive --std=c++0x



bench_server: 
	-mkdir build
	gcc $(CCFLAGS) -o build/bench_server.out $(LIBS) bench_server.c ../../../diversity/c/build/wrapper.o ../../../diversity/c/build/bftsmart-c-server.o -lstdc++ -ljvm

.PHONY: clean

clean:
	rm -rfv *.o *~ *.out build



   


