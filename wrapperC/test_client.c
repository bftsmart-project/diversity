#include <stdio.h>
#include <stdlib.h>
#include "bftsmart-wrapper.h"
#include <string.h>

#define BFT_UNUSED(x) (void)(x)

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("%s","Argumentos invalidos.\n");
        return -1;
    }
    carregarJvm();
    if (createServiceProxy(atoi(argv[1])) != 0) {
        printf("%s","Erro ao criar service proxy.\n");
        return -2;
    }

    const char* stringTeste = "TesteOrdered";
    const char* stringTeste2 = "TesteUnordered";

    char saida[100];
    char saida2[100];

    int result = invokeOrdered((BFT_BYTE*) stringTeste, strlen(stringTeste) + 1, (BFT_BYTE*) saida);
    BFT_UNUSED(result);
    printf("%d%s\n",result, saida);

    int result2 = invokeUnordered((BFT_BYTE*) stringTeste2, strlen(stringTeste2) + 1, (BFT_BYTE*) saida2);
    BFT_UNUSED(result2);
    printf("%d%s\n",result2, saida2);

    finalizarJvm();
}


