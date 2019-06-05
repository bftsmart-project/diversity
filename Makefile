.PHONY : clean all clean-protobuf clean-demo-protobuf demo-c demo-cpp demo-go demo-java demo-python demo-protobuf diversity demo-protobuf-java demo-protobuf-c demo-protobuf-python demo-protobuf-go demo-protobuf-cpp

PASTABUILDPROTO := $(shell pwd)/protobuf
GOPATH = $(shell pwd)/protobuf/protobuf-go
GOPATHARRAY = $(shell pwd)/demos/protobuf/array/build/go
GOPATHKV = $(shell pwd)/demos/protobuf/kv/build/go
GOPATHLIST = $(shell pwd)/demos/protobuf/list2/build/go
GOPATHZERO = $(shell pwd)/demos/protobuf/zero/build/go

all : protobuf/build/mark-proto protobuf/build/mark-protoc protobuf/build/mark-protopy protobuf/build/mark-protoj protobuf/build/mark-protogo diversity demo-c demo-cpp demo-go demo-java demo-python diversity

diversity:
	cd bftsmart; mkdir -p bin; ant
	echo $(MAKEFLAGS)
	cd diversity; $(MAKE) MAKEFLAGS=

include protobuf/Makefile

include demos/protobuf/Makefile

demo-java: protobuf/build/mark-proto diversity demo-protobuf-java
	cd demos/java; $(MAKE) MAKEFLAGS=

demo-c: protobuf/build/mark-protoc diversity demo-protobuf-c
	cd demos/c; $(MAKE) MAKEFLAGS= 

demo-python: protobuf/build/mark-protopy diversity demo-protobuf-python
	cd demos/python; $(MAKE) MAKEFLAGS=

demo-go: protobuf/build/mark-protogo diversity demo-protobuf-go
	cd demos/go; $(MAKE) MAKEFLAGS=

demo-cpp: protobuf/build/mark-proto diversity demo-protobuf-cpp
	cd demos/cpp; $(MAKE) MAKEFLAGS=

clean : clean-protobuf clean-demo-protobuf
	cd bftsmart; rm -rf bin
	cd diversity; $(MAKE) clean MAKEFLAGS=
	cd demos/java; $(MAKE) clean MAKEFLAGS= 
	cd demos/c; $(MAKE) clean MAKEFLAGS=
	cd demos/python; $(MAKE) clean MAKEFLAGS=
	cd demos/go; $(MAKE) clean MAKEFLAGS=
	cd demos/cpp; $(MAKE) clean MAKEFLAGS=
