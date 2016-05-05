all :
	-mkdir build
	../../protobuf/build/bin/protoc-c --c_out=./build Vazio.proto
	../../protobuf/build/bin/protoc --cpp_out=./build Vazio.proto
	../../protobuf/build/bin/protoc --java_out=./build Vazio.proto
	../../protobuf/build/bin/protoc --python_out=./build Vazio.proto


clean :
	rm -rf build

	
