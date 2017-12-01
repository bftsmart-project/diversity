#include <stdio.h>
#include <stdlib.h>
#include <bftsmart-c-server.h>
#include <Request.pb-c.h>
#include <Response.pb-c.h>
#include <Estado.pb-c.h>

#define NAO_UTILIZADA(x) (void)(x)

typedef struct elemento{
    int dado;
    struct elemento * proximo;
}t_elemento;

typedef struct lista{
    t_elemento * inicio;
    t_elemento * fim;
}t_lista;

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
    int i;
    for (i = x-1; i >= 0; i--)
    {
        remover(i,l);
    }
    free (l);
}

//Counter state = COUNTER__INIT;

t_lista * state;

int execute(BFT_BYTE cmd[], int siz, BFT_BYTE ** mem) {
    Bftbench__Request *msg = bftbench__request__unpack(NULL, siz, (const uint8_t*)cmd);
    Bftbench__Response rsp = BFTBENCH__RESPONSE__INIT;
    int x;
    switch (msg->action)
    {
        case BFTBENCH__REQUEST__REQUEST_TYPE__ADD:
            x = buscarIndice(msg->value, state);
            rsp.has_boolresponse = 1;
            rsp.boolresponse = 0;
            if (x < 0) {
              rsp.boolresponse = 1;
              insereFinal(msg->value, state);
            }
            break;
        case BFTBENCH__REQUEST__REQUEST_TYPE__REMOVE:
            x = buscarIndice(msg->value, state);
            rsp.has_boolresponse = 1;
            rsp.boolresponse = 1;
            if (x < 0) rsp.boolresponse = 0;
            else remover(x, state);
            break;
        case BFTBENCH__REQUEST__REQUEST_TYPE__GET:
            rsp.has_intresponse = 1;
            rsp.intresponse = buscarValor(msg->value, state);            
            break;
        case BFTBENCH__REQUEST__REQUEST_TYPE__SIZE:
            rsp.has_intresponse = 1;
            rsp.intresponse = tamanhoLista(state);
            break;
        case BFTBENCH__REQUEST__REQUEST_TYPE__CONTAINS:     
            rsp.has_boolresponse = 1;
            rsp.boolresponse = (buscarIndice(msg->value, state) >= 0);
            break;
        default:
            break;
    }

    unsigned int tamanho = bftbench__response__get_packed_size(&rsp);
    BFT_BYTE * out = (BFT_BYTE*) malloc (tamanho);

    bftbench__response__pack(&rsp, (uint8_t*) out);
    (*mem) = out;
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

    destruirLista(state);

    state = criarLista();
    unsigned int i;
    for ( i = 0; i < msg->n_lista; i++) {
        insereFinal(msg->lista[i], state);
    }


    bftbench__estado__free_unpacked(msg, NULL);

}

void release(BFT_BYTE * mem)
{
  free(mem);
}

int getSnap(BFT_BYTE ** mem) {

    Bftbench__Estado est = BFTBENCH__ESTADO__INIT;

    int x = tamanhoLista(state);
    est.n_lista = x;
    est.lista = (int32_t*) malloc(sizeof(int)  * est.n_lista);
  int i = 0;
  t_elemento * atual = state->inicio;
  while (atual != NULL) {
        est.lista[i] = atual->dado;
i++;
    atual = atual->proximo;
  }


    unsigned int tamanho = bftbench__estado__get_packed_size(&est);

    BFT_BYTE * out = (BFT_BYTE*) malloc (tamanho);

    bftbench__estado__pack(&est, (uint8_t*)out);

    free(est.lista);

    (*mem) = out;

    return tamanho;
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
