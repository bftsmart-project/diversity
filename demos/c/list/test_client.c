#include <stdio.h>
#include <stdlib.h>
#include <bftsmart-c-client.h>
#include <Request.pb-c.h>
#include <Response.pb-c.h>
#include <Estado.pb-c.h>

int main(int argc, char* argv[]) {




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
    if (createServiceProxy(atoi(argv[1])) != 0) {
        printf("%s","Erro ao criar service proxy.\n");
        return -2;
    }
   

    BFT_BYTE saida[10000];

    int cmd,result;
    do {
       printf("%s\n", "1-add");
       printf("%s\n", "2-get");
       printf("%s\n", "3-remove");
       printf("%s\n", "4-contains");
       printf("%s\n", "5-size");
       printf("%s\n", "6-FINALIZAR. pressione ctrl-c depois.");
       scanf("%d",&cmd);
       getchar();
		Bftbench__Request req ;
    unsigned int tamanho ; 
    BFT_BYTE * out ; 

 Bftbench__Response *rsp ; 
       switch (cmd) {

  case 5:
bftbench__request__init(&req);
req.has_value= 0;
req.action = BFTBENCH__REQUEST__REQUEST_TYPE__SIZE;
tamanho = bftbench__request__get_packed_size(&req);
    out = (BFT_BYTE*) malloc (tamanho);

    bftbench__request__pack(&req, (uint8_t*) out);
              result = invokeOrdered(out, tamanho, saida);
rsp = bftbench__response__unpack(NULL, result, (const uint8_t*)saida);
              printf("%s%d\n","response:",rsp->intresponse);
              bftbench__response__free_unpacked(rsp, NULL);
		free(out);
 	      break;
 
  case 2:
		printf("%s\n", "Digite o valor:");
		scanf("%d", &cmd);
		getchar();
bftbench__request__init(&req);
		req.value = cmd;
req.has_value= 1;
req.action = BFTBENCH__REQUEST__REQUEST_TYPE__GET;
tamanho = bftbench__request__get_packed_size(&req);
    out = (BFT_BYTE*) malloc (tamanho);

    bftbench__request__pack(&req, (uint8_t*) out);
              result = invokeOrdered(out, tamanho, saida);
rsp = bftbench__response__unpack(NULL, result, (const uint8_t*)saida);
              printf("%s%d\n","response:",rsp->intresponse);
              bftbench__response__free_unpacked(rsp, NULL);
		free(out);
 	      break;
 
  case 4:
		printf("%s\n", "Digite o valor:");
		scanf("%d", &cmd);
		getchar();
bftbench__request__init(&req);
		req.value = cmd;
req.has_value= 1;
req.action = BFTBENCH__REQUEST__REQUEST_TYPE__CONTAINS;
tamanho = bftbench__request__get_packed_size(&req);
    out = (BFT_BYTE*) malloc (tamanho);

    bftbench__request__pack(&req, (uint8_t*) out);
              result = invokeOrdered(out, tamanho, saida);
rsp = bftbench__response__unpack(NULL, result, (const uint8_t*)saida);
              printf("%s%d\n","response:",rsp->boolresponse);
              bftbench__response__free_unpacked(rsp, NULL);
		free(out);
 	      break;
 
  case 3:
		printf("%s\n", "Digite o valor:");
		scanf("%d", &cmd);
		getchar();
bftbench__request__init(&req);
		req.value = cmd;
req.has_value= 1;
req.action = BFTBENCH__REQUEST__REQUEST_TYPE__REMOVE;
tamanho = bftbench__request__get_packed_size(&req);
    out = (BFT_BYTE*) malloc (tamanho);

    bftbench__request__pack(&req, (uint8_t*) out);
              result = invokeOrdered(out, tamanho, saida);
rsp = bftbench__response__unpack(NULL, result, (const uint8_t*)saida);
              printf("%s%d\n","response:",rsp->boolresponse);
              bftbench__response__free_unpacked(rsp, NULL);
		free(out);
 	      break;
        case 1:
		printf("%s\n", "Digite o valor:");
		scanf("%d", &cmd);
		getchar();
bftbench__request__init(&req);
		req.value = cmd;
req.has_value= 1;
req.action = BFTBENCH__REQUEST__REQUEST_TYPE__ADD;
tamanho = bftbench__request__get_packed_size(&req);
    out = (BFT_BYTE*) malloc (tamanho);

    bftbench__request__pack(&req, (uint8_t*) out);
              result = invokeOrdered(out, tamanho, saida);
rsp = bftbench__response__unpack(NULL, result, (const uint8_t*)saida);
              printf("%s%d\n","response:",rsp->boolresponse);
              bftbench__response__free_unpacked(rsp, NULL);
		free(out);
 	      break;
default:
		break;
       }
    } while (cmd != 6);

    finalizarJvm();
return 0;
}


