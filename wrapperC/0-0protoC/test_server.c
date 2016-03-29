#include <stdio.h>
#include <stdlib.h>
#include <bftsmart-wrapper.h>
#include "testcount.pb-c.h"

#define NAO_UTILIZADA(x) (void)(x)

Counter state = COUNTER__INIT;

int execOrd(BFT_BYTE  cmd[], int siz, BFT_BYTE out[]) {
    NAO_UTILIZADA(cmd);
    NAO_UTILIZADA(siz);
    state.value++;

    printf("%s%d\n", "execOrd:", state.value);

    unsigned int tamanho = counter__get_packed_size(&state);
    counter__pack(&state, (uint8_t*) out);
    return tamanho;
}

int execUnord(BFT_BYTE  cmd[], int siz, BFT_BYTE out[]) {
    NAO_UTILIZADA(cmd);
    NAO_UTILIZADA(siz);

    printf("%s%d\n", "execUnord:", state.value);

    unsigned int tamanho = counter__get_packed_size(&state);
    counter__pack(&state, (uint8_t*) out);
    return tamanho;

}

void installSnap(BFT_BYTE  stateNovo[], int siz) {
    Counter *msg = counter__unpack(NULL, siz, (const uint8_t*)stateNovo);
    state = (*msg);
    printf("%s%d\n", "installSnap:", state.value);
    counter__free_unpacked(msg, NULL);
}

int getSnap(BFT_BYTE  out[]) {
    printf("%s%d\n", "getSnap:", state.value);
    unsigned int tamanho = counter__get_packed_size(&state);
    counter__pack(&state, (uint8_t*)out);
    return tamanho;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("%s", "Argumentos invalidos.\n");
        return -1;
    }

    state.value = 0;

    carregarJvm();
    implementExecuteOrdered(&execOrd);
    implementExecuteUnordered(&execUnord);
    implementInstallSnapshot(&installSnap);
    implementgetSnapshot(&getSnap);

    startServiceReplica(atoi(argv[1]));
    finalizarJvm();
    return 0;
}
