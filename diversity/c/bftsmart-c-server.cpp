#include <jni.h>
#include <iostream>
#include <stdlib.h>
#include <cstdint>
#include <bftsmart_diversity_ServerWrapper.h>
#include "bftsmart-c-server.h"

/* todo: ajustar para overflow nas funcoes do JNI */

/* define utilizado para sinalizar ao compilador
   que alguma variavel realmente nao e utilizada */
#define BFT_UNUSED(x) (void)(x)
#define DEBUG 1



namespace bftsmartdiversity {
    /* todo: em vez de usar classe Java ServerWrapper e ClientWrapper, usar
             diretamente as classes ServiceProxy e ServiceReplica */
    jobject serviceReplica;
    int (*executeOrderedImplementation) (BFT_BYTE [], int, BFT_BYTE **);
    int (*executeUnorderedImplementation) (BFT_BYTE [], int, BFT_BYTE **);
    int (*getSnapshotImplementation) (BFT_BYTE **);
    void (*installSnapshotImplementation) (BFT_BYTE [], int);
    void (*ReleaseGetSnapshotBufferImplementation) (BFT_BYTE *);
    void (*ReleaseExecuteOrderedBufferImplementation) (BFT_BYTE *);
    void (*ReleaseExecuteUnorderedBufferImplementation) (BFT_BYTE *);
    void (*functionCallTestImplementation) (void);
}

extern "C" {

   


    void implementExecuteOrdered(int (*impl) (BFT_BYTE [], int, BFT_BYTE **)) {
        bftsmartdiversity::executeOrderedImplementation = impl;
    }

    void implementExecuteUnordered(int (*impl) (BFT_BYTE [], int, BFT_BYTE **)) {
        bftsmartdiversity::executeUnorderedImplementation = impl;
    }

    void implementInstallSnapshot(void (*impl) (BFT_BYTE [], int)) {
        bftsmartdiversity::installSnapshotImplementation = impl;
    }

    void implementfunctionCallTest(void (*impl) (void)) {
	bftsmartdiversity::functionCallTestImplementation = impl;
    }

    void implementgetSnapshot(int (*impl) (BFT_BYTE **)) {
        bftsmartdiversity::getSnapshotImplementation = impl;
    }

    void implementReleaseGetSnapshotBuffer(void (*impl) (BFT_BYTE*)) {
        bftsmartdiversity::ReleaseGetSnapshotBufferImplementation = impl;
    }

    void implementReleaseExecuteOrderedBuffer(void (*impl) (BFT_BYTE*)) {
        bftsmartdiversity::ReleaseExecuteOrderedBufferImplementation = impl;
    }

    void implementReleaseExecuteUnorderedBuffer(void (*impl) (BFT_BYTE*)) {
        bftsmartdiversity::ReleaseExecuteUnorderedBufferImplementation = impl;
    }


    /*
     * Class:     bftsmart_diversity_ServerWrapper
     * Method:    executeOrderedNative
     * Signature: ([B)[B
     */
    JNIEXPORT jbyteArray JNICALL Java_bftsmart_diversity_ServerWrapper_executeOrderedNative
    (JNIEnv * env, jobject jobj, jbyteArray jbytes) {
        if (DEBUG)
	std::cout << "JNICALL: executeOrderedNative" << std::endl;

        BFT_UNUSED(jobj);
        jint tamanhoN = env->GetArrayLength(jbytes);
        jbyte* nativeArray = env->GetByteArrayElements(jbytes, NULL);

        BFT_BYTE * saida;
        if (DEBUG) {
	std::cout << "executeOrderedImplementation" << std::endl;
        bftsmartdiversity::functionCallTestImplementation();
	std::cout << "test success." << std::endl;
        }
        int tamanhoS = bftsmartdiversity::executeOrderedImplementation((BFT_BYTE*) nativeArray,
                tamanhoN,
                &saida);
        if (DEBUG)
	std::cout << "returned from executeOrderedImplementation" << std::endl;

        jbyteArray arrayJava = env->NewByteArray(tamanhoS);
        env->SetByteArrayRegion(arrayJava, 0, tamanhoS, (jbyte*) saida);
        env->ReleaseByteArrayElements(jbytes, nativeArray, JNI_ABORT);

        bftsmartdiversity::ReleaseExecuteOrderedBufferImplementation(saida);
        if (DEBUG)
	std::cout << "return JNICALL executeOrderedNative" << std::endl;
        return arrayJava;
    }

    /*
     * Class:     bftsmart_diversity_ServerWrapper
     * Method:    executeUnorderedNative
     * Signature: ([B)[B
     */
    JNIEXPORT jbyteArray JNICALL Java_bftsmart_diversity_ServerWrapper_executeUnorderedNative
    (JNIEnv * env, jobject jobj, jbyteArray jbytes) {
        BFT_UNUSED(jobj);
        jint tamanhoN = env->GetArrayLength(jbytes);
        jbyte* nativeArray = env->GetByteArrayElements(jbytes, NULL);

        BFT_BYTE * saida;
        int tamanhoS = bftsmartdiversity::executeUnorderedImplementation((BFT_BYTE*) nativeArray,
                tamanhoN,
                &saida);

        jbyteArray arrayJava = env->NewByteArray(tamanhoS);
        env->SetByteArrayRegion(arrayJava, 0, tamanhoS, (jbyte*) saida);
        env->ReleaseByteArrayElements(jbytes, nativeArray, JNI_ABORT);

        bftsmartdiversity::ReleaseExecuteUnorderedBufferImplementation(saida);
        return arrayJava;
    }

    /*
     * Class:     bftsmart_diversity_ServerWrapper
     * Method:    getSnapshotNative
     * Signature: ()[B
     */
    JNIEXPORT jbyteArray JNICALL Java_bftsmart_diversity_ServerWrapper_getSnapshotNative
    (JNIEnv * env, jobject jobj) {
        BFT_UNUSED(jobj);
        //printf("getSnapshotNative-begin\n");

        BFT_BYTE * saida;
        int tamanhoS = bftsmartdiversity::getSnapshotImplementation(&saida);

        jbyteArray arrayJava = env->NewByteArray(tamanhoS);
        env->SetByteArrayRegion(arrayJava, 0, tamanhoS, (jbyte*) saida);

        bftsmartdiversity::ReleaseGetSnapshotBufferImplementation(saida);
        //printf("getSnapshotNative-end\n");
        return arrayJava;

    }

    /*
     * Class:     bftsmart_diversity_ServerWrapper
     * Method:    installSnapshotNative
     * Signature: ([B)V
     */
    JNIEXPORT void JNICALL Java_bftsmart_diversity_ServerWrapper_installSnapshotNative
    (JNIEnv * env, jobject jobj, jbyteArray jbytes) {
        //printf("installSnapshotNative-begin\n");
        BFT_UNUSED(jobj);
        jint tamanhoN = env->GetArrayLength(jbytes);
        jbyte* nativeArray = env->GetByteArrayElements(jbytes, NULL);

        bftsmartdiversity::installSnapshotImplementation((BFT_BYTE*) nativeArray,
                tamanhoN);
        ;

        env->ReleaseByteArrayElements(jbytes, nativeArray, JNI_ABORT);
        //printf("installSnapshotNative-end\n");
    }

    int startServiceReplica(int id) {
        JavaVM *jvm; /* representa uma VM Java */
        JNIEnv *env; /* ponteiro para a interface de metodos nativos */
        jvm = bftsmartdiversity::jvm;
        env = bftsmartdiversity::env;

        /* invoke the Main.test method using the JNI */
        jclass cls = env->FindClass("bftsmart/diversity/ServerWrapper");

        if (cls == NULL) {
            std::cout << "ERRO no FindClass diversity" << std::endl;
            jvm->DestroyJavaVM();
            return 0x8100;
        }

        const JNINativeMethod methods[] = {

            { (char*) "executeUnorderedNative", (char*) "([B)[B",
                __extension__
                (void*)&Java_bftsmart_diversity_ServerWrapper_executeUnorderedNative},

            { (char*) "executeOrderedNative", (char*) "([B)[B",
                __extension__
                (void*)&Java_bftsmart_diversity_ServerWrapper_executeOrderedNative},

            { (char*) "installSnapshotNative", (char*) "([B)V",
                __extension__
                (void*)&Java_bftsmart_diversity_ServerWrapper_installSnapshotNative},

            { (char*) "getSnapshotNative", (char*) "()[B",
                __extension__
                (void*)&Java_bftsmart_diversity_ServerWrapper_getSnapshotNative}
        };

        const int methods_size = sizeof (methods) / sizeof (methods[0]);

        if (env->RegisterNatives(cls, methods, methods_size) != 0) {
            std::cout << "ERRO no RegisterNatives" << std::endl;
            jvm->DestroyJavaVM();
            return 0x8191;

        }
        jmethodID mid = env->GetMethodID(cls, "<init>", "(I)V");
	if (mid == NULL) {
		std::cout << "nao encontrou construtor" << std::endl;
	}
        jobject newObj = env->NewObject(cls, mid, (jint) id);
         if (newObj == NULL) {
            std::cout << "ERRO ao construir wrapper gmid2:" << id << std::endl;
	    jthrowable exc = env->ExceptionOccurred();
	    if (exc != NULL) {
		env->ExceptionDescribe();


		std::cout << "" << std::endl;
	    }
            jvm->DestroyJavaVM();
            return 0x8201;
        }
        bftsmartdiversity::serviceReplica = newObj;

        /*env->CallStaticVoidMethod(cls, mid, args);*/

        return 0;
    }

} /* extern "C" */


