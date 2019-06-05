protobuf/build/mark-protogo : protobuf/build/mark-proto
	cd protobuf && mkdir build || \
	cd protobuf-go && make && \
	touch build/mark-protogo

protobuf/build/mark-proto :
	#cd protobuf-2.6.1; make check
	cd protobuf && mkdir build || \
	cd protobuf-2.6.1 && ./autogen.sh && \
	./configure --prefix=$(PASTABUILDPROTO)/build && \
	make && \
	make install && \
	cd .. && touch build/mark-proto

protobuf/build/mark-protopy : protobuf/build/mark-proto
	cd protobuf && mkdir build || \
	cd protobuf-2.6.1/python && python setup.py build && \
	echo "FINISHED PROTOPY BUILD." && \
	python setup.py google_test && \
	echo "FINISHED PROTOPY GOOGLE_TEST." && \
	python setup.py install --prefix=$(PASTABUILDPROTO)/build && \
	PYTHONPATH=$(PASTABUILDPROTO)/build/lib/python2.7/site-packages python setup.py install --prefix=$(PASTABUILDPROTO)/build && \
	echo "FINISHED PROTOPY INSTALL." && \
	cd .. && touch build/mark-protopy

protobuf/build/mark-protoj : protobuf/build/mark-proto
	cd protobuf && mkdir build || \
	cd protobuf-2.6.1/java && ../../build/bin/protoc --java_out=src/main/java -I../src ../src/google/protobuf/descriptor.proto && \
	cd .. && javac -d build protobuf-2.6.1/java/src/main/java/com/google/protobuf/*.java && \
	cd build && jar cf protobuf-runtime.jar com && \
	cd .. && touch build/mark-protoj


protobuf/build/mark-protoc : protobuf/build/mark-proto
	cd protobuf && mkdir build || \
	cd protobuf-c && PKG_CONFIG_PATH=$(PASTABUILDPROTO)/build/lib/pkgconfig ./autogen.sh && \
	PKG_CONFIG_PATH=$(PASTABUILDPROTO)/build/lib/pkgconfig ./configure --prefix=$(PASTABUILDPROTO)/build && \
	PKG_CONFIG_PATH=$(PASTABUILDPROTO)/build/lib/pkgconfig make && \
	PKG_CONFIG_PATH=$(PASTABUILDPROTO)/build/lib/pkgconfig make install && \
	cd .. && touch build/mark-protoc

clean-protobuf :
	cd protobuf-2.6.1; make clean
	cd protobuf-c; make clean
	cd protobuf-go; make clean
	rm -rf build
