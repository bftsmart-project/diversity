all :
	-mkdir build
	../../../protobuf/build/bin/protoc-c --c_out=./build Vazio.proto
	../../../protobuf/build/bin/protoc --cpp_out=./build Vazio.proto
	../../../protobuf/build/bin/protoc --java_out=./build Vazio.proto
	../../../protobuf/build/bin/protoc --python_out=./build Vazio.proto
	-mkdir build/classes
	cd build; javac -cp ../../../../protobuf/build/protobuf-runtime.jar -d classes bftbench/*.java
	cd build/classes; jar cf ../vazio-proto.jar bftbench


clean :
	rm -rf build

	
