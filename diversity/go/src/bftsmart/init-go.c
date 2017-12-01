#include <_cgo_export.h>

void init_go()
{
implementExecuteOrdered(&executeOrderedGo);
implementExecuteUnordered(&executeUnorderedGo);
implementgetSnapshot(&getSnapshotGo);
implementfunctionCallTest(&functionCallTestGo);
implementInstallSnapshot(&installSnapshotGo);
implementReleaseExecuteOrderedBuffer(&releaseBufferGo);
implementReleaseExecuteUnorderedBuffer(&releaseBufferGo);
implementReleaseGetSnapshotBuffer(&releaseBufferGo);
}
