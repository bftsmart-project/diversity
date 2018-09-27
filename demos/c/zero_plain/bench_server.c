#include <stdio.h>
#include <stdlib.h>
#include <bftsmart-c-server.h>

#define NAO_UTILIZADA(x) (void)(x)

int execute(BFT_BYTE cmd[], int siz, BFT_BYTE ** mem) {
    printf("--\n");
    printf("Recebido %d bytes\n",siz);
    BFT_BYTE * out = (BFT_BYTE*) malloc (siz);
    memcpy(out, cmd, siz);
    (*mem) = out;
    printf("Enviando %d bytes\n",siz);
    return siz;
}

int execOrd(BFT_BYTE  cmd[], int siz, BFT_BYTE ** out) {
    return execute(cmd,siz,out);
}

int execUnord(BFT_BYTE  cmd[], int siz, BFT_BYTE ** out) {
    return execute(cmd,siz,out);

}

void installSnap(BFT_BYTE  stateNovo[], int siz) {




}

void release(BFT_BYTE * mem)
{
  free(mem);
}

int getSnap(BFT_BYTE ** mem) {



    return 0;
}

void fCallTest() {
	printf("function call test\n");
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
