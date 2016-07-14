#include "ServiceProxy.h"

bftsmart::ServiceProxy::ServiceProxy(int id, string classpath)
{
	setClasspath(classpath.c_str());
	carregarJvm();
	createServiceProxy(id);
}

int bftsmart::ServiceProxy::invokeOrdered(BYTE request[], int size, BYTE response[])
{
	return invokeOrdered(request, size, response);
}

int bftsmart::ServiceProxy::invokeUnordered(BYTE request[], int size, BYTE response[])
{
	return invokeUnordered(request, size, response);
}

bftsmart::ServiceProxy::~ServiceProxy()
{
	finalizarJvm();
}
