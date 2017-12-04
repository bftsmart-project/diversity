#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <Estado.pb.h>
#include <Request.pb.h>
#include <Response.pb.h>
#include <map>
#include "replica.h"

std::map<std::string, std::string> state;

bool buscarlista(int val)
{
    int i = 0;
    for ( auto it = state.begin(); it != state.end(); ++it )
    {
        if ( (*it) == val ) return true;
        i++;
    }
    return false;
}

int elemento_at(int val)
{
    int i = 0;
    for ( auto it = state.begin(); it != state.end(); ++it )
    {
        if ( i == val ) return (*it);
        i++;
    }
    return -1;
}

int execute(BFT_BYTE cmd[], int siz, BFT_BYTE ** mem) {
    using namespace bftbench;

    Request rqst;
    rqst.Clear();

    rqst.ParseFromArray((const void*)cmd, siz);
    Response rsp;
    rsp.Clear();

	std::map<std::string, std::string>::iterator s;
	
    switch (rqst.action())
    {
        case Request::PUT:
            rsp.set_boolresponse(true);
			state[rqst.key()] = rqst.value();
            break;
        case Request::REMOVE:
            rsp.set_boolresponse(false);
			s = state.find(rqst.key());
            if (s != state.end()) {
                rsp.set_boolresponse(true);
                state.erase(rqst.key());
            }
            break;
        case Request::GET:
            rsp.set_boolresponse(false);
			s = state.find(rqst.key());
            if (s != state.end()) {
                rsp.set_boolresponse(true);
                rsp.set_stringresponse(state[rqst.key()]);
            }
            break;
        case Request::LIST:
            rsp.set_boolresponse(true);
			for ( auto it = state.begin(); it != state.end(); ++it )
			{
				rsp.add_listresponse(it->first);
			}
            break;
        default:
            break;
    }

    unsigned int tamanho = rsp.ByteSize();
    BFT_BYTE * out = (BFT_BYTE*) malloc (tamanho);
    (*mem) = out;

    rsp.SerializeToArray((void*) out, tamanho);
    return tamanho;
}

int replica::appExecuteOrdered(BYTE cmd[], int siz, BYTE ** out) {    
	printf("appExecuteOrdered called\n");
    return execute(cmd, siz, out);
}

int replica::executeUnordered(BYTE cmd[], int siz, BYTE ** out) {
		printf("executeUnordered called\n");
     return execute(cmd, siz, out);
}

void replica::installSnapshot(BYTE stateNovo[], int siz) {
    using namespace bftbench;
	printf("installSnapshot called\n");
    Estado est;
    est.Clear();

    est.ParseFromArray((const void *)stateNovo, siz);

    state.clear();

    for (int i = 0; i < est.lista_size(); i++)
    {
        state[est.kv(i).key] = est.kv(i).value;
    }
    
}


int replica::getSnapshot(BYTE ** mem) {
	printf("getSnapshot called\n");
    using namespace bftbench;
    Estado est;
    est.Clear();

    for ( auto it = state.begin(); it != state.end(); ++it )
    {
		MapFieldEntry m;
		m.key = it->first;
		m.value = it->second;
        est.add_kv(m);
    }

    unsigned int tamanho = est.ByteSize();
    BFT_BYTE * out = (BFT_BYTE*) malloc (tamanho);
    (*mem) = out;

    est.SerializeToArray((void*) out, tamanho);
    return tamanho;
}

int main(int argc, char* argv[]) {
   if (argc == 1)
    {
        printf("Usage: %s id_replica classpath_java\n", argv[0]);
        return -1;
    }

    if (argc < 3) {
        printf("%s", "Argumentos invalidos.\n");
        return -1;
    }

    replica * r = new replica(atoi(argv[1]), argv[2]);
	r->startService();
	printf("Acabou.?\n");
	
	while (1) {
		sleep(1);
	}
	
    return 0;
}




