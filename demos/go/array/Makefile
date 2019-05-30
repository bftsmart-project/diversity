## VERSION WARNING
#
# FOR GO VERSION >= 1.9, comment lines with "dummy.c" and uncomment lines with "cp"
#
# FOR GO VERSION <= 1.8, uncommment lines with "dummy.c" and comment lines with "cp".
##

PASTAJAVA := $(shell ../../../scripts/findjava.sh)
INCL=-I$(shell pwd)/../../../diversity/c/headers -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
GOPATH=$(shell pwd)/../../protobuf/array/build/go:$(shell pwd)/../../../protobuf/protobuf-go:$(shell pwd)/../../../diversity/go:$(shell pwd)
CGO_CFLAGS=-g $(INCL)
CGO_LDFLAGS=$(LIBS) -lstdc++ -ljvm

export CGO_CFLAGS
export CGO_LDFLAGS
export GOPATH

.PHONY: all clean server

all: server

server:
	-mkdir build
## VERSION WARNING
#
# FOR GO VERSION >= 1.9, comment lines with "dummy.c" and uncomment lines with "cp"
#
# FOR GO VERSION <= 1.8, uncommment lines with "dummy.c" and comment lines with "cp".
##

	go build -o build/go-server.exe ./go-server

clean:
	rm -rf pkg bin build *~ main


