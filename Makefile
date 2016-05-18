all :
	cd diversity; make
	cd protobuf; make
	cd demos; make


.PHONY : clean

clean : 
	cd diversity; make clean
	cd protobuf; make clean
	cd demos; make clean
