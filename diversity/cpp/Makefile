PASTAJAVA := $(shell ../../scripts/findjava.sh)
INCL=-I../c -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
CCFLAGS=-g -Wextra -Wall 
CPFLAGS=-g -Wextra -Wall -fpermissive --std=c++0x

all: 
	-mkdir build
	g++ $(CPFLAGS) -fPIC $(INCL) $(LIBS) -c *.cpp -ljvm
	mv *.o build/

.PHONY: clean

clean:
	rm -rfv *.o *~ *.out build



   


