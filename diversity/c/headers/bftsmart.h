#ifndef BFTSMART__INCLUDED
#define BFTSMART__INCLUDED

#include <jni.h>

typedef signed char BFT_BYTE;

#ifdef __cplusplus
namespace bftsmartdiversity {
    extern JavaVM *jvm;
    extern JNIEnv *env;
}

extern "C" {
#endif
    void setClasspath(const char* cp);
    int carregarJvm();
   
    void finalizarJvm();
   
    void * bftsmartallocate(size_t tamanho);
    void  bftsmartrelease(void * ponteiro) ;


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BFTSMART__INCLUDED */

