deps := $(wildcard *.java)

build/bftsmart-diversity.jar : $(deps)
	-mkdir build
	javac -cp ../../bftsmart/bin/BFT-SMaRt.jar -d build $(deps)
	cd build; jar cf bftsmart-diversity.jar bftsmart


.PHONY : clean

clean :
	rm -rf build
