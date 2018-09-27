#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <bftsmart-c-server.h>


#define NAO_UTILIZADA(x) (void)(x)

#define DEBUG 0

int resp_size;

int execute(BFT_BYTE cmd[], int siz, BFT_BYTE ** mem) {
    int i;
    printf("--\n");
    printf("Recebido %d bytes\n",siz);

    BFT_BYTE * out = (BFT_BYTE*) malloc (resp_size);
    for (i = 0; i < resp_size; i++) {
           out[i] = 0xff;
    }
    (*mem) = out;

    printf("Enviando %d bytes\n",resp_size);

    return resp_size;
}

int execOrd(BFT_BYTE  cmd[], int siz, BFT_BYTE ** out) {    
    return execute(cmd, siz, out);
}

int execUnord(BFT_BYTE  cmd[], int siz, BFT_BYTE ** out) {
    return execute(cmd, siz, out);

}

void installSnap(BFT_BYTE  stateNovo[], int siz) {
}

int getSnap(BFT_BYTE  ** mem) {
    return 0;
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
        printf("Usage: %s id_replica classpath_java resp_size\n", 
                argv[0]); 
        return -1; 
    }                                                                             
                                                                                  
    if (argc < 4) { 
        printf("%s", "Argumentos invalidos.\n"); 
        return -1; 
    }

    resp_size = atoi(argv[3]);
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

