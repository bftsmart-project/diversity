#include <stdio.h>
#include <stdlib.h>
#include <bftsmart-wrapper.h>
#include "testcount.pb-c.h"

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
    /* todo testar com NULL */
    BFT_BYTE vazio[1];

    BFT_BYTE saida[10000];

    int cmd,result;
    Counter *msg;
    do {
       printf("%s\n", "1-execOrdered");
       printf("%s\n", "2-execUnordered");
       printf("%s\n", "3-FINALIZAR. pressione ctrl-c depois.");
       scanf("%d",&cmd);
       getchar();
       switch (cmd) {
        case 1:
              result = invokeOrdered(vazio, 0, saida);
              msg = counter__unpack(NULL, result, (const uint8_t*) saida);
              printf("%d%s\n",msg->value, ":execOrd<<<<<<<<");
              counter__free_unpacked(msg, NULL);
 	      break;
        case 2:
              result = invokeUnordered(vazio, 0,  saida);
              msg = counter__unpack(NULL, result, (const uint8_t*) saida);
              printf("%d%s\n",msg->value, ":execUnord<<<<<<<");
              counter__free_unpacked(msg, NULL);
              break;
       }
    } while (cmd != 3);

    finalizarJvm();
}


