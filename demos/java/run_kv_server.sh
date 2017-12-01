[ $# -eq 0 ] && { echo "Usage: $0 id_replica"; exit 1; }

#set -x

BASEFOLDER=`pwd`/`dirname $0`

#the current directory , when running this script, must contain a config folder.

#sair imediatamente em caso de erro
set -e

java -cp "$BASEFOLDER/../../bftsmart/library/lib/*:"\
"$BASEFOLDER/../../bftsmart/bin/BFT-SMaRt.jar:"\
"$BASEFOLDER/../../protobuf/build/protobuf-runtime.jar:"\
"$BASEFOLDER/../protobuf/kv/build/kv-proto.jar:"\
"$BASEFOLDER/kv/build/bench_server.jar" bftsmart.diversity.demos.kv.ServerLaunch $1 10000

