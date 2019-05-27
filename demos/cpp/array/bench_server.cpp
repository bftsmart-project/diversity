#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <bftsmart-c-server.h>
#include <Estado.pb.h>
#include <Request.pb.h>
#include <Response.pb.h>


#define NAO_UTILIZADA(x) (void)(x)

#define DEBUG 0

int respsize;

int execute(BFT_BYTE cmd[], int siz, BFT_BYTE ** mem) {
    using namespace bftbench;

    Request rqst;
    rqst.Clear();
    rqst.ParseFromArray((const void*)cmd, siz);
    Response rsp;
    rsp.Clear();
    string * x;
    char * dados = (char*) malloc(respsize);
    x = new string(dados);
    rsp.set_data(&x);	
    unsigned int tamanho = rsp.ByteSize();
    BFT_BYTE * out;
    if (tamanho > 0) {
    	out = (BFT_BYTE*) malloc (tamanho);
    } else {
    	out = (BFT_BYTE*) malloc (1); /* caixa de brita */
    }
    (*mem) = out;

    rsp.SerializeToArray((void*) out, tamanho);
    delete x;
    free(dados) 
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

    Estado est;
    est.Clear();

    est.ParseFromArray((const void *)stateNovo, siz);
}

int getSnap(BFT_BYTE  ** mem) {
    using namespace bftbench;
    Estado est;
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

void fCallTest() {
printf("fcall test\n");
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
    implementfunctionCallTest(&fCallTest);
    implementReleaseGetSnapshotBuffer(&release);
    implementReleaseExecuteOrderedBuffer(&release);
    implementReleaseExecuteUnorderedBuffer(&release);

    startServiceReplica(atoi(argv[1]));
    finalizarJvm();
    return 0;
}

