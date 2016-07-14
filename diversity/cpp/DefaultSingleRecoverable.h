#include "includes.h"

using namespace std;

namespace bftsmart {

class DefaultSingleRecoverable {
public:
	virtual int appExecuteOrdered(BYTE request[], int size, BYTE ** response) = 0;
	virtual int executeUnordered(BYTE request[], int size, BYTE ** response) = 0;
	virtual int getSnapshot(BYTE ** response) = 0;
	virtual void installSnapshot(BYTE state[], int size) = 0;

protected:
	DefaultSingleRecoverable(int id, string classpath);
	~DefaultSingleRecoverable();
};

}
