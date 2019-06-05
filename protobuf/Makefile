protobuf/build/mark-protogo : protobuf/build/mark-proto
	cd protobuf && mkdir -p build && \
	cd protobuf-go && $(MAKE) MAKEFLAGS= && \
	cd .. && touch build/mark-protogo

protobuf/build/mark-proto :
	#cd protobuf-2.6.1; make check
	cd protobuf && mkdir -p build && \
	cd protobuf-2.6.1 && ./autogen.sh && \
	./configure --prefix=$(PASTABUILDPROTO)/build && \
	$(MAKE) && \
	$(MAKE) install MAKEFLAGS= && \
	cd .. && touch build/mark-proto

protobuf/build/mark-protopy : protobuf/build/mark-proto
	cd protobuf && mkdir -p build && \
	cd protobuf-2.6.1/python && python setup.py build && \
	echo "FINISHED PROTOPY BUILD." && \
	PYTHONPATH=$(PASTABUILDPROTO)/build/lib/python2.7/site-packages python setup.py google_test && \
	echo "FINISHED PROTOPY GOOGLE_TEST." && \
	PYTHONPATH=$(PASTABUILDPROTO)/build/lib/python2.7/site-packages python setup.py install --prefix=$(PASTABUILDPROTO)/build && \
	echo "FINISHED PROTOPY INSTALL." && \
	cd ../.. && touch build/mark-protopy

protobuf/build/mark-protoj : protobuf/build/mark-proto
	cd protobuf && mkdir -p build && \
	cd protobuf-2.6.1/java && ../../build/bin/protoc --java_out=src/main/java -I../src ../src/google/protobuf/descriptor.proto && \
	cd ../.. && javac -d build protobuf-2.6.1/java/src/main/java/com/google/protobuf/*.java && \
	cd build && jar cf protobuf-runtime.jar com && \
	cd .. && touch build/mark-protoj


protobuf/build/mark-protoc : protobuf/build/mark-proto
	cd protobuf && mkdir -p build && \
	cd protobuf-c && PKG_CONFIG_PATH=$(PASTABUILDPROTO)/build/lib/pkgconfig ./autogen.sh && \
	PKG_CONFIG_PATH=$(PASTABUILDPROTO)/build/lib/pkgconfig ./configure --prefix=$(PASTABUILDPROTO)/build && \
	PKG_CONFIG_PATH=$(PASTABUILDPROTO)/build/lib/pkgconfig $(MAKE) MAKEFLAGS= && \
	PKG_CONFIG_PATH=$(PASTABUILDPROTO)/build/lib/pkgconfig $(MAKE) install MAKEFLAGS= && \
	cd .. && touch build/mark-protoc

clean-protobuf :
	cd protobuf/protobuf-2.6.1 && $(MAKE) clean MAKEFLAGS=
	cd protobuf/protobuf-c && $(MAKE) clean MAKEFLAGS=
	cd protobuf/protobuf-go && $(MAKE) clean MAKEFLAGS=
	cd protobuf && rm -rf build
