demo-protobuf-java: protobuf/build/mark-protoj
	cd demos/protobuf/array && mkdir -p build && \
	../../../protobuf/build/bin/protoc --java_out=./build *.proto
	cd demos/protobuf/kv && mkdir -p build && \
	../../../protobuf/build/bin/protoc --java_out=./build *.proto
	cd demos/protobuf/list2 && mkdir -p build && \
	../../../protobuf/build/bin/protoc --java_out=./build *.proto
	cd demos/protobuf/zero && mkdir -p build && \
	../../../protobuf/build/bin/protoc --java_out=./build Vazio.proto
	cd demos/protobuf/array && mkdir -p build && \
	mkdir -p build/classes && \
	cd build && javac -cp ../../../../protobuf/build/protobuf-runtime.jar -d classes bftbench/*.java && \
	cd classes && jar cf ../array-proto.jar bftbench
	cd demos/protobuf/kv && mkdir -p build && \
	mkdir -p build/classes && \
	cd build && javac -cp ../../../../protobuf/build/protobuf-runtime.jar -d classes bftbench/*.java && \
	cd classes && jar cf ../kv-proto.jar bftbench
	cd demos/protobuf/list2 && mkdir -p build && \
	mkdir -p build/classes && \
	cd build && javac -cp ../../../../protobuf/build/protobuf-runtime.jar -d classes bftbench/*.java && \
	cd classes && jar cf ../lista-proto.jar bftbench
	cd demos/protobuf/zero && mkdir -p build && \
	mkdir -p build/classes && \
	cd build && javac -cp ../../../../protobuf/build/protobuf-runtime.jar -d classes bftbench/*.java && \
	cd classes && jar cf ../vazio-proto.jar bftbench

demo-protobuf-c: protobuf/build/mark-protoc
	cd demos/protobuf/array && mkdir -p build && \
	../../../protobuf/build/bin/protoc-c --c_out=./build *.proto
	cd demos/protobuf/kv && mkdir -p build && \
	../../../protobuf/build/bin/protoc-c --c_out=./build *.proto
	cd demos/protobuf/list2 && mkdir -p build && \
	../../../protobuf/build/bin/protoc-c --c_out=./build *.proto
	cd demos/protobuf/zero && mkdir -p build && \
	../../../protobuf/build/bin/protoc-c --c_out=./build Vazio.proto

demo-protobuf-python: protobuf/build/mark-protopy
	cd demos/protobuf/array && mkdir -p build && \
	../../../protobuf/build/bin/protoc --python_out=./build *.proto
	cd demos/protobuf/kv && mkdir -p build && \
	../../../protobuf/build/bin/protoc --python_out=./build *.proto
	cd demos/protobuf/list2 && mkdir -p build && \
	../../../protobuf/build/bin/protoc --python_out=./build *.proto
	cd demos/protobuf/zero && mkdir -p build && \
	../../../protobuf/build/bin/protoc --python_out=./build Vazio.proto

demo-protobuf-go: protobuf/build/mark-protogo
	cd demos/protobuf/array && mkdir -p build && \
	mkdir -p build/go/src/bftbench && \
	GOPATH=$(GOPATH):$(GOPATHARRAY) ../../../protobuf/build/bin/protoc --plugin=protoc-gen-go=../../../protobuf/protobuf-go/bin/protoc-gen-go --go_out=./build/go/src/bftbench *.proto && \
	cd build/go && GOPATH=$(GOPATH):$(GOPATHARRAY) go build bftbench && \
	GOPATH=$(GOPATH):$(GOPATHARRAY) go install bftbench
	cd demos/protobuf/kv && mkdir -p build && \
	mkdir -p build/go/src/bftbench && \
	GOPATH=$(GOPATH):$(GOPATHKV) ../../../protobuf/build/bin/protoc --plugin=protoc-gen-go=../../../protobuf/protobuf-go/bin/protoc-gen-go --go_out=./build/go/src/bftbench *.proto && \
	cd build/go && GOPATH=$(GOPATH):$(GOPATHKV) go build bftbench && \
	GOPATH=$(GOPATH):$(GOPATHKV) go install bftbench
	cd demos/protobuf/list2 && mkdir -p build && \
	mkdir -p build/go/src/bftbench && \
	GOPATH=$(GOPATH):$(GOPATHLIST) ../../../protobuf/build/bin/protoc --plugin=protoc-gen-go=../../../protobuf/protobuf-go/bin/protoc-gen-go --go_out=./build/go/src/bftbench *.proto && \
	cd build/go && GOPATH=$(GOPATH):$(GOPATHLIST) go build bftbench && \
	GOPATH=$(GOPATH):$(GOPATHLIST) go install bftbench

demo-protobuf-cpp: protobuf/build/mark-proto
	cd demos/protobuf/array && mkdir -p build && \
	../../../protobuf/build/bin/protoc --cpp_out=./build *.proto
	cd demos/protobuf/kv && mkdir -p build && \
	../../../protobuf/build/bin/protoc --cpp_out=./build *.proto
	cd demos/protobuf/list2 && mkdir -p build && \
	../../../protobuf/build/bin/protoc --cpp_out=./build *.proto
	cd demos/protobuf/zero && mkdir -p build && \
	../../../protobuf/build/bin/protoc --cpp_out=./build Vazio.proto

clean-demo-protobuf:
	cd demos/protobuf/array && rm -rf build
	cd demos/protobuf/kv && rm -rf build
	cd demos/protobuf/zero && rm -rf build
	cd demos/protobuf/list2 && rm -rf build
