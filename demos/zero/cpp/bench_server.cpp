#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <bftsmart-c-server.h>
#include <Vazio.pb.h>


#define NAO_UTILIZADA(x) (void)(x)

#define DEBUG 0


int execute(BFT_BYTE cmd[], int siz, BFT_BYTE ** mem) {
    using namespace bftbench;

    Vazio rqst;
    rqst.Clear();
    rqst.ParseFromArray((const void*)cmd, siz);
    Vazio rsp;
    rsp.Clear();
    unsigned int tamanho = rsp.ByteSize();
    BFT_BYTE * out;
    if (tamanho > 0) {
    	out = (BFT_BYTE*) malloc (tamanho);
    } else {
    	out = (BFT_BYTE*) malloc (1); /* caixa de brita */
    }
    (*mem) = out;

    rsp.SerializeToArray((void*) out, tamanho);
    return tamanho;
}

int execOrd(BFT_BYTE  cmd[], int siz, BFT_BYTE ** out) {    
    return execute(cmd, siz, out);
}

int execUnord(BFT_BYTE  cmd[], int siz, BFT_BYTE ** out) {
    return execute(cmd, siz, out);

}

void installSnap(BFT_BYTE  stateNovo[], int siz) {
    using namespace bftbench;

    Vazio est;
    est.Clear();

    est.ParseFromArray((const void *)stateNovo, siz);
}

int getSnap(BFT_BYTE  ** mem) {
    using namespace bftbench;
    Vazio est;
    est.Clear();

    unsigned int tamanho = est.ByteSize();
    BFT_BYTE * out;
    if (tamanho > 0) {
        out = (BFT_BYTE*) malloc (tamanho);
    } else {
        out = (BFT_BYTE*) malloc (1); /* caixa de brita */
    }
    (*mem) = out;

    est.SerializeToArray((void*) out, tamanho);
    return tamanho;
}

void release(BFT_BYTE * mem)
{
  free(mem);
}

int main(int argc, char* argv[]) {
    if (argc == 1) 
    { 
        printf("Usage: %s id_replica classpath_java\n", 
                argv[0]); 
        return -1; 
    }                                                                             
                                                                                  
    if (argc < 3) { 
        printf("%s", "Argumentos invalidos.\n"); 
        return -1; 
    }


    setClasspath(argv[2]);
    carregarJvm();
    implementExecuteOrdered(&execOrd);
    implementExecuteUnordered(&execUnord);
    implementInstallSnapshot(&installSnap);
    implementgetSnapshot(&getSnap);
    implementReleaseGetSnapshotBuffer(&release);
    implementReleaseExecuteOrderedBuffer(&release);
    implementReleaseExecuteUnorderedBuffer(&release);

    startServiceReplica(atoi(argv[1]));
    finalizarJvm();
    return 0;
}

