#include "includes.h"

using namespace std;

namespace bftsmart {

class ServiceProxy {
public:
	ServiceProxy(int id, string classpath);
	int invokeUnordered(BYTE request[], int size, BYTE response[]);
	int invokeOrdered(BYTE request[], int size, BYTE response[]);
	~ServiceProxy();
};

}
