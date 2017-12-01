.PHONY : clean all

all :
	cd bftsmart; mkdir -p bin; ant
	cd diversity; make
	cd protobuf; make
	cd demos/protobuf; make
	cd demos/java; make
	cd demos/c; make
	cd demos/python; make
	cd demos/go; make
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
