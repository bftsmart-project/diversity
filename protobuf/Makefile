PASTABUILD := $(shell pwd)

all : proto protoc protopy protoj

proto :
	-mkdir build	
	cd protobuf-2.6.1; ./configure --prefix=$(PASTABUILD)/build
	cd protobuf-2.6.1; make
	#cd protobuf-2.6.1; make check
	cd protobuf-2.6.1; make install

protopy : 
	cd protobuf-2.6.1/python; python setup.py build
	echo "FINISHED PROTOPY BUILD."
	cd protobuf-2.6.1/python; python setup.py google_test
	echo "FINISHED PROTOPY GOOGLE_TEST."
	-cd protobuf-2.6.1/python; python setup.py install --prefix=$(PASTABUILD)/build
	cd protobuf-2.6.1/python; PYTHONPATH=$(PASTABUILD)/build/lib/python2.7/site-packages python setup.py install --prefix=$(PASTABUILD)/build
	echo "FINISHED PROTOPY INSTALL."

protoj :
	cd protobuf-2.6.1/java; ../../build/bin/protoc --java_out=src/main/java -I../src ../src/google/protobuf/descriptor.proto
	javac -d build protobuf-2.6.1/java/src/main/java/com/google/protobuf/*.java 
	cd build; jar cf protobuf-runtime.jar com


protoc :
	cd protobuf-c; PKG_CONFIG_PATH=$(PASTABUILD)/build/lib/pkgconfig ./autogen.sh
	cd protobuf-c; PKG_CONFIG_PATH=$(PASTABUILD)/build/lib/pkgconfig ./configure --prefix=$(PASTABUILD)/build
	cd protobuf-c; PKG_CONFIG_PATH=$(PASTABUILD)/build/lib/pkgconfig make
	cd protobuf-c; PKG_CONFIG_PATH=$(PASTABUILD)/build/lib/pkgconfig make install

.PHONY : clean

clean :
	cd protobuf-2.6.1; make clean
	cd protobuf-c; make clean
	rm -rf build

