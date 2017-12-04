#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bftsmart-c-server.h>
#include <Request.pb-c.h>
#include <Response.pb-c.h>
#include <Estado.pb-c.h>
#include "uthash.h"

#define NAO_UTILIZADA(x) (void)(x)

typedef struct my_struct {
    char* key;             
    char* value;
    UT_hash_handle hh;         /* torna a estrutura passível de hash */
} t_hash;

t_hash * t_hash_new() {
    return (t_hash*)malloc(sizeof(t_hash));
}

int key_order(t_hash *a, t_hash *b) {
  return strcmp(a->key,b->key);
}

char* copy_str(char* str) {
    char* mem = (char*)malloc(strlen(str));
    strcpy(mem, str);
    return mem;
}

t_hash * hashset;

int execute(BFT_BYTE cmd[], int siz, BFT_BYTE ** mem) {
    Bftbench__Request *msg = bftbench__request__unpack(NULL, siz, (const uint8_t*)cmd);
    Bftbench__Response rsp = BFTBENCH__RESPONSE__INIT;
    t_hash * s;
    int i;
    
    switch (msg->action)
    {
        case BFTBENCH__REQUEST__REQUEST_TYPE__PUT:
            HASH_FIND(hh, hashset, &(msg->key), strlen(msg->key), s);
            if (s != NULL) {
                free(s->value);
                s->value = copy_str(msg->value);
            } else {
                s = t_hash_new();
                s->key = copy_str(msg->key);
                s->value = copy_str(msg->value);
                HASH_ADD_KEYPTR_INORDER(hh, hashset, s->key, strlen(s->key), s, key_order);            
            }     
            
            rsp.boolresponse = 1;
            break;
        case BFTBENCH__REQUEST__REQUEST_TYPE__DELETE:
            HASH_FIND(hh, hashset, &(msg->key), strlen(msg->key), s);
            if (s != NULL) {
                HASH_DELETE(hh, hashset, s);
                free(s->value);
                free(s->key);
                free(s);
                rsp.boolresponse = 1;
            } else {
                rsp.boolresponse = 0;    
            }                           
            break;
        case BFTBENCH__REQUEST__REQUEST_TYPE__GET:
            HASH_FIND(hh, hashset, &(msg->key), strlen(msg->key), s);
            if (s != NULL) {
                rsp.stringresponse = s->value;
                rsp.boolresponse = 1;
            } else {
                rsp.boolresponse = 0;    
            }                           
            break;
        case BFTBENCH__REQUEST__REQUEST_TYPE__LIST:
            rsp.listresponse = (char**) malloc(HASH_COUNT(hashset));
            rsp.n_listresponse = HASH_COUNT(hashset);
            i = 0;
            for(s=hashset; s != NULL; s=s->hh.next) {
                rsp.listresponse[i++] = s->key; 
            }    
            rsp.boolresponse = 1;
            break;
        default:
            break;
    }
    
    unsigned int tamanho = bftbench__response__get_packed_size(&rsp);
    BFT_BYTE * out = (BFT_BYTE*) malloc (tamanho);
    bftbench__response__pack(&rsp, (uint8_t*) out);
    (*mem) = out;
    if (rsp.n_listresponse > 0) free(rsp.listresponse);
    bftbench__request__free_unpacked(msg,  NULL);
    return tamanho;
}

int execOrd(BFT_BYTE  cmd[], int siz, BFT_BYTE ** out) {
    return execute(cmd,siz,out);
}

int execUnord(BFT_BYTE  cmd[], int siz, BFT_BYTE ** out) {
    return execute(cmd,siz,out);

}

void installSnap(BFT_BYTE  stateNovo[], int siz) {
  
    Bftbench__Estado *msg = bftbench__estado__unpack(NULL, siz, (const uint8_t*)stateNovo);    
    t_hash *curr_item, *tmp;

    HASH_ITER(hh, hashset, curr_item, tmp) {
        HASH_DEL(hashset, curr_item); 
        free(curr_item->key);        
        free(curr_item->value);
        free(curr_item);
    }
    unsigned int i;
    for ( i = 0; i < msg->n_kv; i++) {
        curr_item = t_hash_new();
        curr_item->key = copy_str(msg->kv.key);
        curr_item->value = copy_str(msg->kv.value);
        HASH_ADD_KEYPTR_INORDER(hh, hashset, curr_item->key, strlen(curr_item->key), curr_item, key_order);            
    }
    bftbench__estado__free_unpacked(msg, NULL);
}

void release(BFT_BYTE * mem)
{
  free(mem);
}

int getSnap(BFT_BYTE ** mem) {
    Bftbench__Estado est = BFTBENCH__ESTADO__INIT;
    t_hash * s;
    int x = HASH_COUNT(hashset);
    est.n_kv = x;
    est.kv = (Bftbench__MapFieldEntry*) malloc(sizeof(Bftbench__MapFieldEntry) * x);
    int i = 0;
    for(s=hashset; s != NULL; s=s->hh.next) {        
        est.kv[i].key = s->key; 
        est.kv[i].value = s->value;
    }         
    unsigned int tamanho = bftbench__estado__get_packed_size(&est);
    BFT_BYTE * out = (BFT_BYTE*) malloc (tamanho);
    bftbench__estado__pack(&est, (uint8_t*)out);
    free(est.kv);
    (*mem) = out;
    return tamanho;
}

void fCallTest() {
	printf("function call teste\n");
}

int main(int argc, char* argv[]) {
    hashset = NULL;

    if (argc == 1)
    {
        printf("Usage: %s id_replica classpath_java\n",
                argv[0]);
        return -1;
    }

    if (argc < 3) {
        printf("%s", "Argumentos invalidos.\n");
        return -1;
    }

    setClasspath(argv[2]);

    carregarJvm();
    implementExecuteOrdered(&execOrd);
    implementExecuteUnordered(&execUnord);
    implementInstallSnapshot(&installSnap);
    implementgetSnapshot(&getSnap);
    implementfunctionCallTest(&fCallTest);
    implementReleaseGetSnapshotBuffer(&release);
    implementReleaseExecuteOrderedBuffer(&release);
    implementReleaseExecuteUnorderedBuffer(&release);

    startServiceReplica(atoi(argv[1]));
    finalizarJvm();
    return 0;
}
