#include "includes.h"
#include <bftsmart-c-client.h>

using namespace std;

namespace bftsmart {

class BftSmartClient {
public:
	BftSmartClient(int id, string classpath);
	int invokeUnordered(BYTE request[], int size, BYTE response[]);
	int invokeOrdered(BYTE request[], int size, BYTE response[]);
	~BftSmartClient();
};

}
