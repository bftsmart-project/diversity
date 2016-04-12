#include <jni.h>
#include <iostream>
#include <stdlib.h>
#include <cstdint>
#include "bftsmart_tom_diversity_ServerWrapper.h"
#include "bftsmart-wrapper.h"

/* todo: ajustar para overflow nas funcoes do JNI */

/* define utilizado para sinalizar ao compilador
   que alguma variavel realmente nao e utilizada */
#define BFT_UNUSED(x) (void)(x)
#define DEBUG 0

typedef signed char BFT_BYTE;

namespace bftsmartdiversity {
    /* todo: em vez de usar classe Java ServerWrapper e ClientWrapper, usar
             diretamente as classes ServiceProxy e ServiceReplica */
    jobject serviceProxy;
    jobject serviceReplica;
    JavaVM *jvm;
    JNIEnv *env;
    jmethodID executeOrderedMethod;
    jmethodID executeUnorderedMethod;
    /* classpath padrao: execucao direto de dentro do diretorio do bft-smart */
    const char * classpath = "-Djava.class.path=dist/BFT-SMaRt.jar:lib/slf4j-api-1.5.8.jar:lib/slf4j-jdk14-1.5.8.jar:lib/netty-3.1.1.GA.jar:lib/commons-codec-1.5.jar";
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

    void setClasspath(const char* cp) {
        bftsmartdiversity::classpath = cp;
    }

    int carregarJvm() {
        JavaVMInitArgs vm_args; /* argumentos de inicializacao da JVM */
        JavaVMOption* options = new JavaVMOption[1];

        options[0].optionString = (char*) bftsmartdiversity::classpath;

        vm_args.version = JNI_VERSION_1_6; /* 1_6 mesmo, mesmo para Java 7 */
        vm_args.nOptions = 1;
        vm_args.options = options;
        vm_args.ignoreUnrecognized = false;
        /* load and initialize a Java VM, return a JNI interface
         * pointer in env */
        JNI_CreateJavaVM(&bftsmartdiversity::jvm, (void**) &bftsmartdiversity::env, &vm_args);
        delete options;
	return 0;
    }

    int createServiceProxy(int id) {
        JavaVM *jvm; /* representa uma VM Java */
        JNIEnv *env; /* ponteiro para a interface de metodos nativos */
        jvm = bftsmartdiversity::jvm;
        env = bftsmartdiversity::env;

        /* invoke the Main.test method using the JNI */
        jclass cls = env->FindClass("bftsmart/tom/diversity/ClientWrapper");

        if (cls == NULL) {
            std::cout << "ERRO no FindClass diversity" << std::endl;
            jvm->DestroyJavaVM();
            return 0x8100;
        }

        jint arg = (jint) id;
        jmethodID constru = env->GetMethodID(cls, "<init>", "(I)V");
        if (constru == NULL) {
            std::cout << "ERRO ao construir wrapper" << std::endl;
            jvm->DestroyJavaVM();
            return 0x8200;
        }

        bftsmartdiversity::serviceProxy = env->NewObject(cls, constru, arg);
        if (bftsmartdiversity::serviceProxy == NULL) {
            std::cout << "ERRO ao construir wrapper" << std::endl;
            jvm->DestroyJavaVM();
            return 0x8201;
        }

        bftsmartdiversity::executeOrderedMethod = env->GetMethodID(cls, "executeOrdered", "([B)[B");
        bftsmartdiversity::executeUnorderedMethod = env->GetMethodID(cls, "executeUnordered", "([B)[B");
        if (bftsmartdiversity::executeOrderedMethod == NULL || bftsmartdiversity::executeUnorderedMethod == NULL) {
            std::cout << "ERRO ao obter execute(un) ordered  " << std::endl;
            jvm->DestroyJavaVM();
            return 0x8107;
        }

        return 0x0;
    }

 void * bftsmartallocate(size_t tamanho) {
void * alocad = malloc(tamanho);
if (DEBUG)
std::cout << "alocando - " << tamanho << "--" << (int64_t)alocad << std::endl;
return malloc(tamanho);
 }
    void  bftsmartrelease(void * ponteiro) {
        free(ponteiro);
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
                bftsmartdiversity::executeOrderedMethod, arrayJava));

        if (result == NULL) {
            std::cout << "erro o chamar execute ordered" << std::endl;
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
                bftsmartdiversity::executeUnorderedMethod, arrayJava));

        if (result == NULL) {
            std::cout << "erro o chamar execute unordered" << std::endl;
            jvm->DestroyJavaVM();
            return 0x8109;
        }
        jint saidaT = env->GetArrayLength(result);
        env->GetByteArrayRegion(result, 0, saidaT, (jbyte*) saida);
        return (int) saidaT;
    }

    void finalizarJvm() {
        /* todo: descarregar corretamente os objetos do java (serviceproxy e servicereplica) */
        bftsmartdiversity::jvm->DestroyJavaVM();
    }

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
     * Class:     bftsmart_tom_diversity_ServerWrapper
     * Method:    executeOrderedNative
     * Signature: ([B)[B
     */
    JNIEXPORT jbyteArray JNICALL Java_bftsmart_tom_diversity_ServerWrapper_executeOrderedNative
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
     * Class:     bftsmart_tom_diversity_ServerWrapper
     * Method:    executeUnorderedNative
     * Signature: ([B)[B
     */
    JNIEXPORT jbyteArray JNICALL Java_bftsmart_tom_diversity_ServerWrapper_executeUnorderedNative
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
     * Class:     bftsmart_tom_diversity_ServerWrapper
     * Method:    getSnapshotNative
     * Signature: ()[B
     */
    JNIEXPORT jbyteArray JNICALL Java_bftsmart_tom_diversity_ServerWrapper_getSnapshotNative
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
     * Class:     bftsmart_tom_diversity_ServerWrapper
     * Method:    installSnapshotNative
     * Signature: ([B)V
     */
    JNIEXPORT void JNICALL Java_bftsmart_tom_diversity_ServerWrapper_installSnapshotNative
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
        jclass cls = env->FindClass("bftsmart/tom/diversity/ServerWrapper");

        if (cls == NULL) {
            std::cout << "ERRO no FindClass diversity" << std::endl;
            jvm->DestroyJavaVM();
            return 0x8100;
        }

        const JNINativeMethod methods[] = {

            { (char*) "executeUnorderedNative", (char*) "([B)[B",
                __extension__
                (void*)&Java_bftsmart_tom_diversity_ServerWrapper_executeUnorderedNative},

            { (char*) "executeOrderedNative", (char*) "([B)[B",
                __extension__
                (void*)&Java_bftsmart_tom_diversity_ServerWrapper_executeOrderedNative},

            { (char*) "installSnapshotNative", (char*) "([B)V",
                __extension__
                (void*)&Java_bftsmart_tom_diversity_ServerWrapper_installSnapshotNative},

            { (char*) "getSnapshotNative", (char*) "()[B",
                __extension__
                (void*)&Java_bftsmart_tom_diversity_ServerWrapper_getSnapshotNative}
        };

        const int methods_size = sizeof (methods) / sizeof (methods[0]);

        if (env->RegisterNatives(cls, methods, methods_size) != 0) {
            std::cout << "ERRO no RegisterNatives" << std::endl;
            jvm->DestroyJavaVM();
            return 0x8191;

        }
        jmethodID mid = env->GetMethodID(cls, "<init>", "(I)V");
        jobject newObj = env->NewObject(cls, mid, (jint) id);
        if (newObj == NULL) {
            std::cout << "ERRO ao construir wrapper" << std::endl;
            jvm->DestroyJavaVM();
            return 0x8201;
        }
        bftsmartdiversity::serviceReplica = newObj;

        /*env->CallStaticVoidMethod(cls, mid, args);*/

        return 0;
    }

} /* extern "C" */


