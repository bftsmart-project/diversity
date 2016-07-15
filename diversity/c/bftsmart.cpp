#include <jni.h>
#include <iostream>
#include <stdlib.h>
#include <cstdint>
#include "bftsmart.h"

/* todo: ajustar para overflow nas funcoes do JNI */

/* define utilizado para sinalizar ao compilador
   que alguma variavel realmente nao e utilizada */
#define BFT_UNUSED(x) (void)(x)
#define DEBUG 0


namespace bftsmartdiversity {
    JavaVM *jvm;
    JNIEnv *env;
    /* classpath padrao: execucao direto de dentro do diretorio do bft-smart */
    const char * classpath = "-Djava.class.path=dist/BFT-SMaRt.jar:lib/slf4j-api-1.5.8.jar:lib/slf4j-jdk14-1.5.8.jar:lib/netty-3.1.1.GA.jar:lib/commons-codec-1.5.jar";
   
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

 void * bftsmartallocate(size_t tamanho) {
void * alocad = malloc(tamanho);
if (DEBUG)
std::cout << "alocando - " << tamanho << "--" << (int64_t)alocad << std::endl;
return malloc(tamanho);
 }
    void  bftsmartrelease(void * ponteiro) {
        free(ponteiro);
    }



    void finalizarJvm() {
        /* todo: descarregar corretamente os objetos do java (serviceproxy e servicereplica) */
        bftsmartdiversity::jvm->DestroyJavaVM();
    }


} /* extern "C" */


