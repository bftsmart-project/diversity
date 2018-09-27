#include "bftsmart-client.hpp"

bftsmart::BftSmartClient::BftSmartClient(int id, string classpath)
{
	setClasspath(classpath.c_str());
	carregarJvm();
	createServiceProxy(id);
}

int bftsmart::BftSmartClient::invokeOrdered(BYTE request[], int size, BYTE response[])
{
	return invokeOrdered(request, size, response);
}

int bftsmart::BftSmartClient::invokeUnordered(BYTE request[], int size, BYTE response[])
{
	return invokeUnordered(request, size, response);
}

bftsmart::BftSmartClient::~BftSmartClient()
{
	finalizarJvm();
}
