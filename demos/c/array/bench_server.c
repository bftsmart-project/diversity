#include <stdio.h>
#include <stdlib.h>
#include <bftsmart-c-server.h>
#include <Estado.pb-c.h>
#include <Response.pb-c.h>
#include <Request.pb-c.h>
#include <time.h>

#define NAO_UTILIZADA(x) (void)(x)
int respsize;
int interval;
struct timespec start_time;
int stt_time_set;
int rqst_count;

int execute(BFT_BYTE cmd[], int siz, BFT_BYTE ** mem) {
    if (!stt_time_set) {
	    clock_gettime(CLOCK_MONOTONIC, &start_time);
	    stt_time_set = 1;
    }
    Bftbench__Request *msg = bftbench__request__unpack(NULL, siz, (const uint8_t*)cmd);
    Bftbench__Response rsp = BFTBENCH__RESPONSE__INIT;
    uint8_t * dado = (uint8_t*) malloc(respsize);
    struct timespec now;

    rsp.data.data = dado;
    rsp.data.len = respsize;
    unsigned int tamanho = bftbench__response__get_packed_size(&rsp);
    BFT_BYTE * out = (BFT_BYTE*) malloc (tamanho);

    bftbench__response__pack(&rsp, (uint8_t*) out);
    (*mem) = out;
    bftbench__request__free_unpacked(msg, NULL) ;
    if (rqst_count >= interval) {
	    clock_gettime(CLOCK_MONOTONIC, &now);
	    printf("Throughput: %f / s", rqst_count / difftime(now.tv_sec, start_time.tv_sec));
	    clock_gettime(CLOCK_MONOTONIC, &start_time);
	    rqst_count = 0;

    }
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


    bftbench__estado__free_unpacked(msg, NULL);

}

void release(BFT_BYTE * mem)
{
  free(mem);
}

int getSnap(BFT_BYTE ** mem) {

    Bftbench__Estado est = BFTBENCH__ESTADO__INIT;

    unsigned int tamanho = bftbench__estado__get_packed_size(&est);

    BFT_BYTE * out = (BFT_BYTE*) malloc (tamanho);

    bftbench__estado__pack(&est, (uint8_t*)out);

    (*mem) = out;

    return tamanho;
}

void fCallTest() {
	printf("function call test\n");
}

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
    implementExecuteOrdered(&execOrd);
    implementExecuteUnordered(&execUnord);
    implementInstallSnapshot(&installSnap);
    implementgetSnapshot(&getSnap);
    implementfunctionCallTest(&fCallTest);
    implementReleaseGetSnapshotBuffer(&release);
    implementReleaseExecuteOrderedBuffer(&release);
    implementReleaseExecuteUnorderedBuffer(&release);

    respsize = atoi(argv[3]);
    interval = atoi(argv[4]);
    rqst_count = 0;
    stt_time_set = 0;
    startServiceReplica(atoi(argv[1]));
    finalizarJvm();
    return 0;
}
