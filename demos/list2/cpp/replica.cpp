#include <stdio.h>
#include <stdlib.h>
#include <Estado.pb.h>
#include <Request.pb.h>
#include <Response.pb.h>
#include <list>
#include "replica.h"

/* estado da replica. neste caso, a lista */
std::list<int> state;

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

    switch (rqst.action())
    {
        case Request::ADD:
            rsp.set_boolresponse(true);
            if (buscarlista(rqst.value())) rsp.set_boolresponse(false);
            else state.insert(state.end(), rqst.value());
            break;
        case Request::REMOVE:
            rsp.set_boolresponse(false);
            if (buscarlista(rqst.value())) {
                rsp.set_boolresponse(true);
                state.remove(rqst.value());
            }
            break;
        case Request::GET:
            rsp.set_intresponse(elemento_at(rqst.value()));
            break;
        case Request::SIZE:
            rsp.set_intresponse(state.size());
            break;
        case Request::CONTAINS:
            rsp.set_boolresponse(buscarlista(rqst.value()));
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
    return execute(cmd, siz, out);
}

int replica::executeUnordered(BYTE cmd[], int siz, BYTE ** out) {
     return execute(cmd, siz, out);
}

void replica::installSnapshot(BYTE stateNovo[], int siz) {
    using namespace bftbench;

    Estado est;
    est.Clear();

    est.ParseFromArray((const void *)stateNovo, siz);

    state.clear();

    for (int i = 0; i < est.lista_size(); i++)
    {
        state.insert(state.end(), est.lista(i));
    }

    
}


int replica::getSnapshot(BYTE ** mem) {
    using namespace bftbench;
    Estado est;
    est.Clear();

    for ( auto it = state.begin(); it != state.end(); ++it )
    {
        est.add_lista(*it);
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

    replica r(atoi(argv[1]), argv[2]);
    return 0;
}




