GOPATH = $(shell pwd)/build/go:$(shell pwd)/../../../protobuf/protobuf-go

export GOPATH

all :
	-mkdir build
	../../../protobuf/build/bin/protoc-c --c_out=./build *.proto
	../../../protobuf/build/bin/protoc --cpp_out=./build *.proto
	../../../protobuf/build/bin/protoc --java_out=./build *.proto
	../../../protobuf/build/bin/protoc --python_out=./build *.proto
	-mkdir -p build/go/src/bftbench
	../../../protobuf/build/bin/protoc --plugin=protoc-gen-go=../../../protobuf/protobuf-go/bin/protoc-gen-go --go_out=./build/go/src/bftbench *.proto
	cd build/go; go build bftbench; 
	cd build/go; go install bftbench; 
	-mkdir build/classes
	cd build; javac -cp ../../../../protobuf/build/protobuf-runtime.jar -d classes bftbench/*.java
	cd build/classes; jar cf ../lista-proto.jar bftbench


clean :
	rm -rf build

	
