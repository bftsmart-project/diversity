#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <bftsmart-c-server.h>
#include <Estado.pb.h>
#include <Request.pb.h>
#include <Response.pb.h>
#include <time.h>


#define NAO_UTILIZADA(x) (void)(x)

#define DEBUG 0

int respsize;
int interval;
struct timespec start_time;
int stt_time_set;
int rqst_count;
double max_tp;

int
timespec_subtract (struct timespec *result, struct timespec *x, struct timespec *y)
{
	          /* Perform the carry for the later subtraction by updating y. */
	          if (x->tv_nsec < y->tv_nsec) {
			                        int nsec = (y->tv_nsec - x->tv_nsec) / 1000000000 + 1;
						                          y->tv_nsec -= 1000000000 * nsec;
									                                y->tv_sec += nsec;
													                                }
		              if (x->tv_nsec - y->tv_nsec > 1000000000) {
				                              int nsec = (x->tv_nsec - y->tv_nsec) / 1000000000;
							                                  y->tv_nsec += 1000000000 * nsec;
											                                  y->tv_sec -= nsec;
															                                    }

			                    /* Compute the time remaining to wait.
					     *                *      tv_usec is certainly positive. */
			                    result->tv_sec = x->tv_sec - y->tv_sec;
					                    result->tv_nsec = x->tv_nsec - y->tv_nsec;

							                      /* Return 1 if result is negative. */
							                      return x->tv_sec < y->tv_sec;
}



int execute(BFT_BYTE cmd[], int siz, BFT_BYTE ** mem) {
    if (!stt_time_set) {
	    clock_gettime(CLOCK_MONOTONIC, &start_time);
	    stt_time_set = 1;
    }	    
    using namespace bftbench;

    Request rqst;
    rqst.Clear();
    rqst.ParseFromArray((const void*)cmd, siz);
    Response rsp;
    rsp.Clear();
    std::string * x;
    std::cout << respsize << std::endl;
    char * dados = (char*) malloc(respsize == 0 ? 1 : respsize);
    dados[0] = 0xFF;
    struct timespec now;
    x = new std::string(dados, respsize);
    rsp.set_data(*x);	
    unsigned int tamanho = rsp.ByteSize();
    std::cout << tamanho << std::endl;
    BFT_BYTE * out;
    if (tamanho > 0) {
    	out = (BFT_BYTE*) malloc (tamanho);
    } else {
    	out = (BFT_BYTE*) malloc (1); /* caixa de brita */
	out[0] = 0xFF;
    }
    (*mem) = out;
    rqst_count++;
if (rqst_count >= interval) {
clock_gettime(CLOCK_MONOTONIC, &now);
            struct timespec diff;
	                timespec_subtract(&diff, &now, &start_time);
			double tp = rqst_count / (diff.tv_sec + diff.tv_nsec / 1000000000.0);
			            printf("Throughput: %f / s", rqst_count / (diff.tv_sec + diff.tv_nsec / 1000000000.0));

				    if (tp > max_tp) {
					    max_tp = tp;
				    }
			            printf("Max Throughput: %f / s", max_tp);
				                clock_gettime(CLOCK_MONOTONIC, &start_time);
						            rqst_count = 0;

}
							    
    rsp.SerializeToArray((void*) out, tamanho);
    delete x;
    free(dados); 
    return tamanho > 0 ? tamanho : 1;
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
}

int getSnap(BFT_BYTE  ** mem) {
    using namespace bftbench;
    Estado est;
    est.Clear();

    unsigned int tamanho = est.ByteSize();
    BFT_BYTE * out;
    if (tamanho > 0) {
        out = (BFT_BYTE*) malloc (tamanho);
    } else {
        out = (BFT_BYTE*) malloc (1); /* caixa de brita */
    }
    (*mem) = out;

    est.SerializeToArray((void*) out, tamanho);
    return tamanho;
}

void release(BFT_BYTE * mem)
{
  free(mem);
}

void fCallTest() {
printf("fcall test\n");
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

respsize = atoi(argv[3]);
max_tp = 0;
		interval = atoi(argv[4]);
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
rqst_count = 0;
    stt_time_set = 0;
    startServiceReplica(atoi(argv[1]));
    finalizarJvm();
    return 0;
}

