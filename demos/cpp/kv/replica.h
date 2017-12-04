#include <bftsmart-server.hpp>
#include <string>

class replica : public bftsmart::BftSmartServer
{
public:
        int appExecuteOrdered(BYTE request[], int size, BYTE ** response);
	int executeUnordered(BYTE request[], int size, BYTE ** response);
	int getSnapshot(BYTE ** response);
	void installSnapshot(BYTE state[], int size);

        replica(int id, string classpath) : BftSmartServer(id , classpath) { }
};
