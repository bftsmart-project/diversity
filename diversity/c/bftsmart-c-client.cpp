#include <jni.h>
#include <iostream>
#include <stdlib.h>
#include <cstdint>
#include "bftsmart-c-client.h"

/* todo: ajustar para overflow nas funcoes do JNI */

/* define utilizado para sinalizar ao compilador
   que alguma variavel realmente nao e utilizada */
#define BFT_UNUSED(x) (void)(x)
#define DEBUG 0



namespace bftsmartdiversity {
    /* todo: em vez de usar classe Java ServerWrapper e ClientWrapper, usar
             diretamente as classes ServiceProxy e ServiceReplica */
    jobject serviceProxy;

    jmethodID invokeOrderedMethod;
    jmethodID invokeUnorderedMethod;
   
}

extern "C" {


    int createServiceProxy(int id) {
        JavaVM *jvm; /* representa uma VM Java */
        JNIEnv *env; /* ponteiro para a interface de metodos nativos */
        jvm = bftsmartdiversity::jvm;
        env = bftsmartdiversity::env;

        /* invoke the Main.test method using the JNI */
        jclass cls = env->FindClass("bftsmart/diversity/ClientWrapper");

        if (cls == NULL) {
            std::cout << "ERRO no FindClass diversity" << std::endl;
            jvm->DestroyJavaVM();
            return 0x8100;
        }

        jint arg = (jint) id;
        jmethodID constru = env->GetMethodID(cls, "<init>", "(I)V");
        if (constru == NULL) {
            std::cout << "ERRO ao construir wrapper getmid" << std::endl;
            jvm->DestroyJavaVM();
            return 0x8200;
        }

        bftsmartdiversity::serviceProxy = env->NewObject(cls, constru, arg);
        if (bftsmartdiversity::serviceProxy == NULL) {
            std::cout << "ERRO ao construir wrapper int. srvprox" << std::endl;
            jvm->DestroyJavaVM();
            return 0x8201;
        }

        bftsmartdiversity::invokeOrderedMethod = env->GetMethodID(cls, "invokeOrdered", "([B)[B");
        bftsmartdiversity::invokeUnorderedMethod = env->GetMethodID(cls, "invokeUnordered", "([B)[B");
        if (bftsmartdiversity::invokeOrderedMethod == NULL || bftsmartdiversity::invokeUnorderedMethod == NULL) {
            std::cout << "ERRO ao obter execute(un) ordered  " << std::endl;
            jvm->DestroyJavaVM();
            return 0x8107;
        }

        return 0x0;
    }



    // todo: checar overflow .

    int invokeOrdered(BFT_BYTE command[], int tamanho, BFT_BYTE saida[]) {
        JavaVM *jvm; /* representa uma VM Java */
        JNIEnv *env; /* ponteiro para a interface de metodos nativos */
        jvm = bftsmartdiversity::jvm;
        env = bftsmartdiversity::env;

        /* apenas para fins de debug
        printf("tamanho: %d\n",tamanho);
        for (int i=0;i<tamanho;i++) 
        {
            printf("%x ",command[i]);
        }
        printf("\n");
         fim do debug*/

        jbyteArray arrayJava = env->NewByteArray(tamanho);
        env->SetByteArrayRegion(arrayJava, 0, tamanho, (jbyte*) command);

        jbyteArray result = (jbyteArray) (env->CallObjectMethod(bftsmartdiversity::serviceProxy,
                bftsmartdiversity::invokeOrderedMethod, arrayJava));

        if (result == NULL) {
            std::cout << "erro o chamar invoke ordered" << std::endl;
            jvm->DestroyJavaVM();
            return 0x8109;
        }
        jint saidaT = env->GetArrayLength(result);
        env->GetByteArrayRegion(result, 0, saidaT, (jbyte*) saida);

        /* apenas para fins de debug
        printf("tamanho: %d\n",saidaT);
        for (int i=0;i<saidaT;i++) 
        {
            printf("%x ",saida[i]);
        }
        printf("\n");
        fim do debug */


        return (int) saidaT;
    }

    // todo: checar overflow .

    int invokeUnordered(BFT_BYTE command[], int tamanho, BFT_BYTE saida[]) {
        JavaVM *jvm; /* representa uma VM Java */
        JNIEnv *env; /* ponteiro para a interface de metodos nativos */
        jvm = bftsmartdiversity::jvm;
        env = bftsmartdiversity::env;

        jbyteArray arrayJava = env->NewByteArray(tamanho);
        env->SetByteArrayRegion(arrayJava, 0, tamanho, (jbyte*) command);

        jbyteArray result = (jbyteArray) (env->CallObjectMethod(bftsmartdiversity::serviceProxy,
                bftsmartdiversity::invokeUnorderedMethod, arrayJava));

        if (result == NULL) {
            std::cout << "erro o chamar invoke unordered" << std::endl;
            jvm->DestroyJavaVM();
            return 0x8109;
        }
        jint saidaT = env->GetArrayLength(result);
        env->GetByteArrayRegion(result, 0, saidaT, (jbyte*) saida);
        return (int) saidaT;
    }

} /* extern "C" */


