PASTAJAVA := $(shell ../../scripts/findjava.sh)
INCL=-I../../../c -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
CGO_CFLAGS=-g -Wextra -Wall $(INCL)
CGO_LDFLAGS=../../../c/build/wrapper.o ../../../c/build/bftsmart-c-client.o ../../../c/build/bftsmart-c-server.o $(LIBS) -lstdc++ -ljvm
GOPATH = $(shell pwd)

export CGO_CFLAGS
export CGO_LDFLAGS
export GOPATH

all: 
	go build bftsmart bftsmartclient bftsmartserver
	go install bftsmart bftsmartclient bftsmartserver

.PHONY: clean

clean:
	rm -rf pkg/* bin/*;



   


