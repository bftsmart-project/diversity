PASTAJAVA := $(shell ../../../scripts/findjava.sh)
INCL=-I$(shell pwd)/../../../diversity/c -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
GOPATH = $(shell pwd)/../../protobuf/list2/build/go:$(shell pwd)/../../../protobuf/protobuf-go:$(shell pwd)/../../../diversity/go:$(shell pwd)
CGO_CFLAGS=-g -Wextra -Wall $(INCL)
CGO_LDFLAGS=$(shell pwd)/../../../diversity/c/build/wrapper.o $(shell pwd)/../../../diversity/c/build/bftsmart-c-client.o $(shell pwd)/../../../diversity/c/build/bftsmart-c-server.o $(LIBS) -lstdc++ -ljvm
#CGO_LDFLAGS=$(shell pwd)/../../../diversity/c/build/wrapper.o $(shell pwd)/../../../diversity/c/build/bftsmart-c-client.o $(LIBS) -lstdc++ -ljvm
#CGO_LDFLAGS=$(LIBS) -lstdc++ -ljvm

export CGO_CFLAGS
export CGO_LDFLAGS
export GOPATH

.PHONY: all clean client server

all: client server

client:
	-mkdir build
## VERSION WARNING
#
# FOR GO VERSION >= 1.9, comment lines with "dummy.c" and uncomment lines with "cp"
#
# FOR GO VERSION < 1.9, uncommment lines with "dummy.c" and comment lines with "cp".
##
	gcc -o build/wrapper.o -c dummy.c
	gcc -o build/bftsmart-c-client.o -c dummy.c
	gcc -o build/bftsmart-c-server.o -c dummy.c
#	cp -v ../../../diversity/c/build/wrapper.o build/
#	cp -v ../../../diversity/c/build/bftsmart-c-client.o build/
#	cp -v ../../../diversity/c/build/bftsmart-c-server.o build/
	go build -o build/go-client.exe ./go-client

server:
	-mkdir build
## VERSION WARNING
#
# FOR GO VERSION >= 1.9, comment lines with "dummy.c" and uncomment lines with "cp"
#
# FOR GO VERSION < 1.9, uncommment lines with "dummy.c" and comment lines with "cp".
##

	gcc -o build/wrapper.o -c dummy.c
	gcc -o build/bftsmart-c-client.o -c dummy.c
	gcc -o build/bftsmart-c-server.o -c dummy.c
#	cp -v ../../../diversity/c/build/wrapper.o build/
#	cp -v ../../../diversity/c/build/bftsmart-c-client.o build/
#	cp -v ../../../diversity/c/build/bftsmart-c-server.o build/
	go build -o build/go-server.exe ./go-server

clean:
	rm -rf pkg bin build *~ main


