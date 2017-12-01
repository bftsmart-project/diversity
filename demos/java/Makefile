.PHONY : clean all

all :
	mkdir -p bin; pwd; ant -Dlib.dir=../../bftsmart/library/lib -Ddist.dir=../../bftsmart/bin -Dprotomsg.dir=../protobuf/zero/build -Dprotort.dir=../../protobuf/build
	mkdir -p bin; pwd; ant -f build-kv.xml -Dlib.dir=../../bftsmart/library/lib -Ddist.dir=../../bftsmart/bin -Dprotomsg.dir=../protobuf/kv/build -Dprotort.dir=../../protobuf/build
	cd list2; make
	cd list; make
	cd kv; make

clean :
	rm -rf bin
	cd list2; make clean
	cd list; make
	cd kv; make clean
