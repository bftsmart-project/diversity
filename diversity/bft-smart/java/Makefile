all : 
	-mkdir build 
	javac -cp ../../lib/BFT-SMaRt.jar -d build src/bftsmart/diversity/*.java
	-mkdir dist
	cd build; jar cf ../dist/bftsmart-diversity.jar bftsmart

clean :
	rm -rf build
	rm -rf dist
