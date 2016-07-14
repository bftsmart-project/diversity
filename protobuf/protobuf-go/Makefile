GOPATH = $(shell pwd)
export GOPATH
all :
	cd src/github.com/golang/protobuf; go install ./proto ./jsonpb ./ptypes 
	cd src/github.com/golang/protobuf; go install ./protoc-gen-go

clean :
	rm -rf bin pkg
