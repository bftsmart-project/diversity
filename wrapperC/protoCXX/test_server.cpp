#include <stdio.h>
#include <stdlib.h>
#include <bftsmart-wrapper.h>
#include "testcount.pb.h"

#define NAO_UTILIZADA(x) (void)(x)

Counter state;

int execOrd(BFT_BYTE  cmd[], int siz, BFT_BYTE out[]) {
    NAO_UTILIZADA(cmd);
    NAO_UTILIZADA(siz);
    state.set_value(state.value() + 1);

    printf("%s%d\n", "execOrd:", state.value());

    unsigned int tamanho = state.ByteSize();
    state.SerializeToArray((void*) out, tamanho);
    return tamanho;
}

int execUnord(BFT_BYTE  cmd[], int siz, BFT_BYTE out[]) {
    NAO_UTILIZADA(cmd);
    NAO_UTILIZADA(siz);

    printf("%s%d\n", "execUnord:", state.value());

    unsigned int tamanho = state.ByteSize();
    state.SerializeToArray((void*) out, tamanho);
    return tamanho;

}

void installSnap(BFT_BYTE  stateNovo[], int siz) {
    state.ParseFromArray((const void *)stateNovo, siz);
    printf("%s%d\n", "installSnap:", state.value());
}

int getSnap(BFT_BYTE  out[]) {
    printf("%s%d\n", "getSnap:", state.value());
    unsigned int tamanho = state.ByteSize();
    state.SerializeToArray((void*) out, tamanho);
    return tamanho;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("%s", "Argumentos invalidos.\n");
        return -1;
    }
    state.Clear();
    state.set_value(0);

    carregarJvm();
    implementExecuteOrdered(&execOrd);
    implementExecuteUnordered(&execUnord);
    implementInstallSnapshot(&installSnap);
    implementgetSnapshot(&getSnap);

    startServiceReplica(atoi(argv[1]));
    finalizarJvm();
    return 0;
}
