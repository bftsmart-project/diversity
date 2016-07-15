#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <bftsmart-c-server.h>
#include "Estado.pb.h"
#include "Request.pb.h"
#include "Response.pb.h"
#include <list>


#define NAO_UTILIZADA(x) (void)(x)

#define DEBUG 0

typedef struct elemento{
    int dado;
    struct elemento * proximo;
}t_elemento;

typedef struct lista{
    t_elemento * inicio;
    t_elemento * fim;
}t_lista;


t_lista * state;

int insereInicio(int valor, t_lista * l){
    t_elemento * novoprimeiro = (t_elemento *)malloc(sizeof(t_elemento));
    novoprimeiro->dado = valor;
    novoprimeiro->proximo = l->inicio;
    l->inicio = novoprimeiro;
    if(l->fim == NULL){ //ou novoprimeiro->proximo == NULL ou l->inicio->proximo == NULL
       l->fim = l->inicio;
    }
    return 0;
}

int insereFinal(int valor, t_lista * l){
    t_elemento * novoultimo = (t_elemento *)malloc(sizeof(t_elemento));
    novoultimo->dado = valor;
    novoultimo->proximo = NULL;
    if(l->inicio == NULL){
        l->inicio = novoultimo; 
    }else{
        l->fim->proximo = novoultimo;
    
    }
    l->fim = novoultimo;
    return 0;
}

int estaVazia(t_lista * l){
    if(l->inicio == NULL){
          return 1;
    }
    return 0;
}


int removeInicio(t_lista * l){
    if(l->inicio == NULL){
       return -1;
    }
    int tmp = l->inicio->dado;
    t_elemento * removido = l->inicio;
    l->inicio = l->inicio->proximo;
    free(removido);
    if(l->inicio == NULL){
       l->fim = NULL;

     }
     return tmp;
}


int removeFinal(t_lista * l){
    if(l->inicio == NULL){
       return -1;
    }
    if(l->inicio == l->fim){
       int tmp = l->inicio->dado;
       free(l->inicio);
       l->inicio = NULL;
       l->fim = NULL;
       return tmp;  
    }
    t_elemento * penultimo = l->inicio;
    while(penultimo->proximo != l->fim){ //penultimo->proximo->proximo !=NULL
       penultimo = penultimo->proximo;
    }
    int tmp = penultimo->proximo->dado;
    free(l->fim);
    l->fim = penultimo;
    penultimo->proximo = NULL;
    return tmp;
}


int remover(int pos, t_lista * l){
    if(l->inicio == NULL){
       return -1;
    }
    if(pos < 0){
        return -1;
    }
    t_elemento * removido = l->inicio;
    t_elemento * ant_removido = NULL;
    int i = 0;
    for(i=0;i<pos && removido != NULL;i++){
       ant_removido = removido;
       removido = removido->proximo;
    } 
    if(removido != NULL){
       if(removido == l->inicio){
          l->inicio = removido->proximo;
        }else{
          ant_removido->proximo = removido->proximo;
        }
        if(removido == l->fim){
           l->fim = ant_removido;

         } 


       int tmp = removido->dado;
       
       free(removido);
        return tmp;

    }else{
       return -1;
    }


}

int buscarIndice(int valor, t_lista * l) {
  t_elemento * atual = l->inicio;
  int i = 0;
  while (atual != NULL) {
    if (atual->dado == valor) return i;
    i++;
    atual = atual->proximo;
  }
  return -1;
}

int buscarValor(int pos, t_lista * l) {
  t_elemento * atual = l->inicio;
  int i = 0;
  while (atual != NULL) {
    if (i == pos) return atual->dado;
    i++;
    atual = atual->proximo;
  }
  return -1;
}

int tamanhoLista(t_lista * l) {
  t_elemento * atual = l->inicio;
  int i = 0;
  while (atual != NULL) {    
    i++;
    atual = atual->proximo;
  }
  return i;
}

int inserir(int pos, int valor, t_lista * l){
    if(pos <= 0){
       insereInicio(valor,l);
    }else{   
       t_elemento * atual = l->inicio;
       int i = 0;
       for(i = 0; i < (pos-1) && atual != NULL; i++){
           atual = atual->proximo;
       }
       if(atual == NULL || atual == l->fim){
           insereFinal(valor,l);
       }else{
           t_elemento * novo = (t_elemento *) malloc(sizeof(t_elemento));
           novo->dado = valor;
           novo->proximo = atual->proximo;
       atual->proximo = novo;
       }

       

    }
    return 0;
}

t_lista * criarLista()
{
  t_lista * l = (t_lista *)malloc(sizeof(t_lista));
  l->inicio = NULL;
  l->fim = NULL;
  return l;
}

void destruirLista(t_lista * l)
{
    int x =tamanhoLista(l);
    for (int i = x-1; i >= 0; i--)
    {
        remover(i,l);
    }
    free (l);
}


int execute(BFT_BYTE cmd[], int siz, BFT_BYTE ** mem) {
    using namespace bftbench;

    Request rqst;
    rqst.Clear();
	int x;
    rqst.ParseFromArray((const void*)cmd, siz);
    Response rsp;
    rsp.Clear();
if (DEBUG) {
    std::cout << "action:" << rqst.action() << std::endl;
    std::cout << "value:" << rqst.value() << std::endl; }
    switch (rqst.action())
    {
        case Request::ADD:
            rsp.set_boolresponse(true);
            if (buscarIndice(rqst.value(), state) >= 0) 
{ rsp.set_boolresponse(false);
if(DEBUG) std::cout << "rsp:false" << std::endl;
}
            else 
{ insereFinal(rqst.value(), state);
if (DEBUG) std::cout << "rsp:true" << std::endl;

}
            break;            
        case Request::REMOVE:
            rsp.set_boolresponse(false);
	    x = buscarIndice(rqst.value(), state);
            if (x >= 0) {
                rsp.set_boolresponse(true);
                remover(x, state);
if (DEBUG) std::cout << "rsp:true" << std::endl;

            } else { 
if (DEBUG) std::cout << "rsp:false" << std::endl;

}
            break;
        case Request::GET:
            rsp.set_intresponse(buscarValor(rqst.value(), state));
            break;
        case Request::SIZE:
            rsp.set_intresponse(tamanhoLista(state));
            break;
        case Request::CONTAINS:
            rsp.set_boolresponse(buscarIndice(rqst.value(), state) >= 0);
            break;
        default:
            break;
    }

    unsigned int tamanho = rsp.ByteSize();
    BFT_BYTE * out = (BFT_BYTE*) malloc (tamanho);
    (*mem) = out;

    rsp.SerializeToArray((void*) out, tamanho);
    return tamanho;
}

int execOrd(BFT_BYTE  cmd[], int siz, BFT_BYTE ** out) {    
    return execute(cmd, siz, out);
}

int execUnord(BFT_BYTE  cmd[], int siz, BFT_BYTE ** out) {
    return execute(cmd, siz, out);

}

void installSnap(BFT_BYTE  stateNovo[], int siz) {
    using namespace bftbench;

    Estado est;
    est.Clear();

    est.ParseFromArray((const void *)stateNovo, siz);

    destruirLista(state);

    state = criarLista();
    for (int i = 0; i < est.lista_size(); i++) {
        insereFinal(est.lista(i), state);
    }
}

int getSnap(BFT_BYTE  ** mem) {
    using namespace bftbench;
    Estado est;
    est.Clear();

  t_elemento * atual = state->inicio;
  while (atual != NULL) {
    est.add_lista(atual->dado);
    atual = atual->proximo;
  }

    unsigned int tamanho = est.ByteSize();
    BFT_BYTE * out = (BFT_BYTE*) malloc (tamanho);
    (*mem) = out;

    est.SerializeToArray((void*) out, tamanho);
    return tamanho;
}

void release(BFT_BYTE * mem)
{
  free(mem);
}

int main(int argc, char* argv[]) {
  state = criarLista();

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
    implementReleaseGetSnapshotBuffer(&release);
    implementReleaseExecuteOrderedBuffer(&release);
    implementReleaseExecuteUnorderedBuffer(&release);

    startServiceReplica(atoi(argv[1]));
    finalizarJvm();
    return 0;
}

