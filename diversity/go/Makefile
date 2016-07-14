PASTAJAVA := $(shell ../../scripts/findjava.sh)
INCL=-I../../../c -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
CGO_CFLAGS=-g -Wextra -Wall $(INCL)
CGO_LDFLAGS=../../../c/build/wrapper.o $(LIBS) -lstdc++ -ljvm
GOPATH = $(shell pwd)

export CGO_CFLAGS
export CGO_LDFLAGS
export GOPATH

all: 
	go build bftsmart
	go install bftsmart

.PHONY: clean

clean:
	rm -rf pkg/* bin/*;



   


