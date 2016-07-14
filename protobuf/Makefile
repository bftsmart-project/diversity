PASTABUILD := $(shell pwd)

.PHONY : all clean build

all : build 

build : build/mark-proto build/mark-protoc build/mark-protopy build/mark-protoj build/mark-protogo

build/mark-protogo :
	-mkdir build
	cd protobuf-go; make
	touch build/mark-protogo

build/mark-proto :
	-mkdir build	
	cd protobuf-2.6.1; ./autogen.sh
	cd protobuf-2.6.1; ./configure --prefix=$(PASTABUILD)/build
	cd protobuf-2.6.1; make
	#cd protobuf-2.6.1; make check
	cd protobuf-2.6.1; make install
	touch build/mark-proto

build/mark-protopy : build/mark-proto
	-mkdir build	
	cd protobuf-2.6.1/python; python setup.py build
	echo "FINISHED PROTOPY BUILD."
	cd protobuf-2.6.1/python; python setup.py google_test
	echo "FINISHED PROTOPY GOOGLE_TEST."
	-cd protobuf-2.6.1/python; python setup.py install --prefix=$(PASTABUILD)/build
	cd protobuf-2.6.1/python; PYTHONPATH=$(PASTABUILD)/build/lib/python2.7/site-packages python setup.py install --prefix=$(PASTABUILD)/build
	echo "FINISHED PROTOPY INSTALL."
	touch build/mark-protopy

build/mark-protoj : build/mark-proto
	-mkdir build	
	cd protobuf-2.6.1/java; ../../build/bin/protoc --java_out=src/main/java -I../src ../src/google/protobuf/descriptor.proto
	javac -d build protobuf-2.6.1/java/src/main/java/com/google/protobuf/*.java 
	cd build; jar cf protobuf-runtime.jar com
	touch build/mark-protoj


build/mark-protoc : build/mark-proto
	-mkdir build	
	cd protobuf-c; PKG_CONFIG_PATH=$(PASTABUILD)/build/lib/pkgconfig ./autogen.sh
	cd protobuf-c; PKG_CONFIG_PATH=$(PASTABUILD)/build/lib/pkgconfig ./configure --prefix=$(PASTABUILD)/build
	cd protobuf-c; PKG_CONFIG_PATH=$(PASTABUILD)/build/lib/pkgconfig make
	cd protobuf-c; PKG_CONFIG_PATH=$(PASTABUILD)/build/lib/pkgconfig make install
	touch build/mark-protoc

clean :
#	cd protobuf-2.6.1; make clean
	cd protobuf-c; make clean
	cd protobuf-go; make clean
	rm -rf build

