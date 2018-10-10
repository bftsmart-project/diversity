PASTAJAVA := $(shell ../../scripts/findjava.sh)
INCL=-I$(shell pwd)/../c/headers -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
#CGO_CFLAGS=-g -Wextra -Wall $(INCL)
CGO_CFLAGS=-g $(INCL)
CGO_LDFLAGS=$(LIBS) -lstdc++ -ljvm
GOPATH = $(shell pwd)

export CGO_CFLAGS
export CGO_LDFLAGS
export GOPATH

all: 
	-mkdir build
	-mkdir src/bftsmart/build
	-mkdir src/bftsmartserver/build
	-mkdir src/bftsmartclient/build
	cp ../c/build/wrapper.o src/bftsmart/build/
	cp ../c/build/bftsmart-c-client.o src/bftsmart/build/
	cp ../c/build/bftsmart-c-server.o src/bftsmart/build/
	go install -a -buildmode=archive bftsmart bftsmartclient bftsmartserver

.PHONY: clean

clean:
	rm -rf pkg bin build;
	cd src/bftsmart; rm -rf build;
	cd src/bftsmartserver; rm -rf build;
	cd src/bftsmartclient; rm -rf build;



   


