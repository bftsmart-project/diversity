.PHONY : clean all protobuf demo-c demo-cpp demo-go demo-java demo-python diversity

unexport MAKEFLAGS

all : protobuf diversity demo-c demo-cpp demo-go demo-java demo-python diversity

diversity:
	cd bftsmart; mkdir -p bin; ant
	cd diversity; make

protobuf:
	cd protobuf; make
	cd demos/protobuf; make

demo-java: protobuf diversity
	cd demos/java; make

demo-c: protobuf diversity
	cd demos/c; make

demo-python: protobuf diversity
	cd demos/python; make

demo-go: protobuf diversity
	cd demos/go; make

demo-cpp: protobuf diversity
	cd demos/cpp; make

clean :
	cd bftsmart; rm -rf bin
	cd diversity; make clean
	cd demos/protobuf; make clean
	cd protobuf; make clean
	cd demos/java; make clean
	cd demos/c; make clean
	cd demos/python; make clean
	cd demos/go; make clean
	cd demos/cpp; make clean
