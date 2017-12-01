#include <stdio.h>
#include <stdlib.h>
#include <bftsmart-c-server.h>
#include <Vazio.pb-c.h>

#define NAO_UTILIZADA(x) (void)(x)

int execute(BFT_BYTE cmd[], int siz, BFT_BYTE ** mem) {
    printf("--\n");
    printf("Recebido %d bytes\n",siz);
    Bftbench__Vazio *msg = bftbench__vazio__unpack(NULL, siz, (const uint8_t*)cmd);
    Bftbench__Vazio rsp = BFTBENCH__VAZIO__INIT;
    unsigned int tamanho = bftbench__vazio__get_packed_size(&rsp);
    BFT_BYTE * out = (BFT_BYTE*) malloc (tamanho);

    bftbench__vazio__pack(&rsp, (uint8_t*) out);
    (*mem) = out;
    printf("Enviando %d bytes\n",tamanho);
    return tamanho;
}

int execOrd(BFT_BYTE  cmd[], int siz, BFT_BYTE ** out) {
    return execute(cmd,siz,out);
}

int execUnord(BFT_BYTE  cmd[], int siz, BFT_BYTE ** out) {
    return execute(cmd,siz,out);

}

void installSnap(BFT_BYTE  stateNovo[], int siz) {

    Bftbench__Vazio *msg = bftbench__vazio__unpack(NULL, siz, (const uint8_t*)stateNovo);    


    bftbench__vazio__free_unpacked(msg, NULL);

}

void release(BFT_BYTE * mem)
{
  free(mem);
}

int getSnap(BFT_BYTE ** mem) {

    Bftbench__Vazio est = BFTBENCH__VAZIO__INIT;

    unsigned int tamanho = bftbench__vazio__get_packed_size(&est);

    BFT_BYTE * out = (BFT_BYTE*) malloc (tamanho);

    bftbench__vazio__pack(&est, (uint8_t*)out);

    (*mem) = out;

    return tamanho;
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
