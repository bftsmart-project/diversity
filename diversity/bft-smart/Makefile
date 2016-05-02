all :	
	-mkdir dist
	cd java; make
	cp -rf java/dist/bftsmart-diversity.jar dist
	cd intermediate; make
	cp -rf intermediate/dist .

clean :
	cd java; make clean
	cd intermediate; make clean
	rm -rfv dist

