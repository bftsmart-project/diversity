#ifndef BFTSMART_SERVER_INCLUDED
#define BFTSMART_SERVER_INCLUDED

#include <jni.h>
#include "bftsmart.h"

#ifdef __cplusplus
extern "C" {
#endif
    void implementExecuteOrdered(int (*impl) (BFT_BYTE [], int, BFT_BYTE **));
    void implementExecuteUnordered(int (*impl) (BFT_BYTE [], int, BFT_BYTE **));
    void implementInstallSnapshot(void (*impl) (BFT_BYTE [], int));
    void implementgetSnapshot(int (*impl) (BFT_BYTE **));
    int startServiceReplica(int id);
    void implementReleaseExecuteOrderedBuffer(void (*impl) (BFT_BYTE*));
    void implementReleaseExecuteUnorderedBuffer(void (*impl) (BFT_BYTE*));
    void implementReleaseGetSnapshotBuffer(void (*impl) (BFT_BYTE*));
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BFTSMART_SERVER_INCLUDED */

