#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bftsmart-wrapper.h"
#define BFT_UNUSED(x) (void)(x)

int execOrd(BFT_BYTE  cmd[], int siz, BFT_BYTE out[]) {
    printf("%s", "execOrd\n");
/* inverte string */
    for (int i = 1; i < siz; i++)
        out[i-1] = cmd[siz-i-1];
    out[siz-1] = '\0';
    return siz;
}

int execUnord(BFT_BYTE  cmd[], int siz, BFT_BYTE out[]) {
    printf("%s", "execUnord\n");
/* inverte string */
    for (int i = 1; i < siz; i++)
        out[i-1] = cmd[siz-i-1];
    out[siz-1] = '\0';
    return siz;
}

void installSnap(BFT_BYTE  state[], int siz) {
    printf("%s", "installSnap\n");
    BFT_UNUSED(state);
    BFT_UNUSED(siz);
}

int getSnap(BFT_BYTE  out[]) {
    printf("%s", "getSnap\n");
    BFT_UNUSED(out);

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("%s", "Argumentos invalidos.\n");
        return -1;
    }
    carregarJvm();
    implementExecuteOrdered(&execOrd);
    implementExecuteUnordered(&execUnord);
    implementInstallSnapshot(&installSnap);
    implementgetSnapshot(&getSnap);

    startServiceReplica(atoi(argv[1]));
    finalizarJvm();
    return 0;
}
