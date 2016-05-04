PASTABUILD := $(shell pwd)

all : proto protoc


proto :
	-mkdir build	
	cd protobuf-2.6.1; ./configure --prefix=$(PASTABUILD)/build
	cd protobuf-2.6.1; make
	cd protobuf-2.6.1; make check
	cd protobuf-2.6.1; make install


protoc : PKG_CONFIG_PATH=$(PASTABUILD)/build/lib/pkgconfig
protoc :
	cd protobuf-c; ./autogen.sh
	cd protobuf-c; ./configure --prefix=$(PASTABUILD)/build
	cd protobuf-c; make
	cd protobuf-c; make install

.PHONY : clean

clean :
	cd protobuf-2.6.1; make clean
	cd protobuf-c; make clean
	rm -rf build

