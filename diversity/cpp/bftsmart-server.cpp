#include "bftsmart-server.hpp"

namespace bftsmart
{
class FunctionCall {
public:
    static bftsmart::BftSmartServer* instance;
    static int callExecOrd(BYTE req[], int sz, BYTE**resp);
    static int callExecUnord(BYTE req[], int sz, BYTE**resp);
    static int callgetsnap(BYTE** resp);
    static void callinstall(BYTE stt[], int sz);
    static void callRelease(BYTE mem[]);
	static void fCallTest() { 
		printf("fCallTest called\n");
	}
};
}

bftsmart::BftSmartServer* bftsmart::FunctionCall::instance;

int bftsmart::FunctionCall::callExecOrd(BYTE req[], int sz, BYTE**resp) {
	printf("bftsmart::FunctionCall::callExecOrd\n");
	return instance->appExecuteOrdered(req,sz,resp);
}

int bftsmart::FunctionCall::callExecUnord(BYTE req[], int sz, BYTE**resp) {
	return instance->executeUnordered(req,sz,resp);
}

int bftsmart::FunctionCall::callgetsnap(BYTE** resp) {
	return instance->getSnapshot(resp);
}

void bftsmart::FunctionCall::callinstall(BYTE stt[], int sz) {
	instance->installSnapshot(stt,sz);
}

void bftsmart::FunctionCall::callRelease(BYTE mem[]) {
	free(mem);
}

bftsmart::BftSmartServer::BftSmartServer(int id, string classpath)
{
	setClasspath(classpath.c_str());
	carregarJvm();    
	this->replicaId = id;
	
	implementfunctionCallTest(&bftsmart::FunctionCall::fCallTest);
	implementExecuteOrdered(&bftsmart::FunctionCall::callExecOrd);
	implementExecuteUnordered(&bftsmart::FunctionCall::callExecUnord);
	implementgetSnapshot(&bftsmart::FunctionCall::callgetsnap);
	implementInstallSnapshot(&bftsmart::FunctionCall::callinstall);
	implementReleaseGetSnapshotBuffer(&bftsmart::FunctionCall::callRelease);
	implementReleaseExecuteOrderedBuffer(&bftsmart::FunctionCall::callRelease);
	implementReleaseExecuteUnorderedBuffer(&bftsmart::FunctionCall::callRelease);
	
	/* startServiceReplica(id); */
}

void bftsmart::BftSmartServer::startService()
{
	bftsmart::FunctionCall::instance = this;
	
	startServiceReplica(this->replicaId);
}

bftsmart::BftSmartServer::~BftSmartServer()
{
	finalizarJvm();
}
