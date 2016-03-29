#ifndef BFTSMART_WRAPPER_INCLUDED
#define BFTSMART_WRAPPER_INCLUDED

#include <jni.h>

typedef signed char BFT_BYTE;

#ifdef __cplusplus
extern "C" {
#endif
    void setClasspath(const char* cp);
    int carregarJvm();
    int createServiceProxy(int id);
    int invokeOrdered(BFT_BYTE command[], int tamanho, BFT_BYTE saida[]);
    int invokeUnordered(BFT_BYTE command[], int tamanho, BFT_BYTE saida[]);
    void finalizarJvm();
    void implementExecuteOrdered(int (*impl) (BFT_BYTE [], int, BFT_BYTE **));
    void implementExecuteUnordered(int (*impl) (BFT_BYTE [], int, BFT_BYTE **));
    void implementInstallSnapshot(void (*impl) (BFT_BYTE [], int));
    void implementgetSnapshot(int (*impl) (BFT_BYTE **));
    int startServiceReplica(int id);
    void implementReleaseExecuteOrderedBuffer(void (*impl) (BFT_BYTE*));
    void implementReleaseExecuteUnorderedBuffer(void (*impl) (BFT_BYTE*));
    void implementReleaseGetSnapshotBuffer(void (*impl) (BFT_BYTE*));
    void * bftsmartallocate(size_t tamanho);
    void  bftsmartrelease(void * ponteiro) ;
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BFTSMART_WRAPPER_INCLUDED */

