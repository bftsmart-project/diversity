PASTABUILD := $(shell pwd)

all : proto protoc


proto :
	-mkdir build	
	cd protobuf-2.6.1; ./configure --prefix=$(PASTABUILD)/build
	cd protobuf-2.6.1; make
	cd protobuf-2.6.1; make check
	cd protobuf-2.6.1; make install


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

