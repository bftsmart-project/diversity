#ifndef BFTSMART_CLIENT_INCLUDED
#define BFTSMART_CLIENT_INCLUDED

#include <jni.h>
#include "bftsmart.h"

typedef signed char BFT_BYTE;

#ifdef __cplusplus
extern "C" {
#endif
   
    int createServiceProxy(int id);
    int invokeOrdered(BFT_BYTE command[], int tamanho, BFT_BYTE saida[]);
    int invokeUnordered(BFT_BYTE command[], int tamanho, BFT_BYTE saida[]);
   
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BFTSMART_CLIENT_INCLUDED */

