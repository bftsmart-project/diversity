INCL=-I/home/caio/repositorios-git/tcc-2015/wrapperC -I/usr/lib/jvm/java-7-openjdk-i386/include/ -I/usr/lib/jvm/java-7-openjdk-i386/include/linux
LIBS=-L /usr/lib/jvm/java-7-openjdk-i386/jre/lib/i386/client -ljvm
INCL += `pkg-config --cflags --libs libprotobuf-c protobuf | sed 's/-lprotobuf-c//' | sed 's/-lprotobuf//'`
CCFLAGS=-std=c99 -pedantic -g -Wextra -Wall -Werror
CPFLAGS=-std=c++11 -pedantic -g -Wextra -Wall -Werror
SFX=/home/caio/repositorios-git/tcc-2015/prefix/lib/libprotobuf-c.a
SFX += /home/caio/repositorios-git/tcc-2015/prefix/lib/libprotobuf.a

wrapper:
	g++ $(CPFLAGS) -o wrapper.o $(INCL) $(LIBS) -c ../bftsmart-wrapper.cpp

test_client: wrapper
	gcc $(CCFLAGS) -o test_client.out $(INCL) $(LIBS) testcount.pb-c.c test_client.c wrapper.o $(SFX)

test_server: wrapper
	gcc $(CCFLAGS) -o test_server.out $(INCL) $(LIBS) testcount.pb-c.c test_server.c wrapper.o $(SFX)

bench_server: wrapper
	gcc $(CCFLAGS) -o bench_server.out $(INCL) $(LIBS) Request.pb-c.c Response.pb-c.c Estado.pb-c.c bench_server.c wrapper.o $(SFX)


tests: test_server test_client

all: wrapper tests bench_server

.PHONY: clean

clean:
	rm -fv *.o *~ *.out



   


