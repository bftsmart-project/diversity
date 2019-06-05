.PHONY : clean all clean-protobuf clean-demo-protobuf demo-c demo-cpp demo-go demo-java demo-python demo-protobuf diversity demo-protobuf-java demo-protobuf-c demo-protobuf-python demo-protobuf-go demo-protobuf-cpp

PASTABUILDPROTO := $(shell pwd)/protobuf
GOPATH = $(shell pwd)/protobuf/protobuf-go
GOPATHARRAY = $(shell pwd)/demos/protobuf/array/build/go
GOPATHKV = $(shell pwd)/demos/protobuf/kv/build/go
GOPATHLIST = $(shell pwd)/demos/protobuf/list2/build/go
GOPATHZERO = $(shell pwd)/demos/protobuf/zero/build/go

unexport MAKEFLAGS

all : protobuf/build/mark-proto protobuf/build/mark-protoc protobuf/build/mark-protopy protobuf/build/mark-protoj protobuf/build/mark-protogo diversity demo-c demo-cpp demo-go demo-java demo-python diversity

diversity:
	cd bftsmart; mkdir -p bin; ant
	cd diversity; make

include protobuf/Makefile

include demos/protobuf/Makefile

demo-java: protobuf/build/mark-proto diversity demo-protobuf-java
	cd demos/java; make

demo-c: protobuf/build/mark-protoc diversity demo-protobuf-c
	cd demos/c; make

demo-python: protobuf/build/mark-protopy diversity demo-protobuf-python
	cd demos/python; make

demo-go: protobuf/build/mark-protogo diversity demo-protobuf-go
	cd demos/go; make

demo-cpp: protobuf/build/mark-proto diversity demo-protobuf-cpp
	cd demos/cpp; make

clean : clean-protobuf clean-demo-protobuf
	cd bftsmart; rm -rf bin
	cd diversity; make clean
	cd demos/java; make clean
	cd demos/c; make clean
	cd demos/python; make clean
	cd demos/go; make clean
	cd demos/cpp; make clean
