[ $# -eq 0 ] && { echo "Usage: $0 id_replica"; exit 1; }

#set -x

BASEFOLDER=`pwd`/`dirname $0`

#the current directory , when running this script, must contain a config folder.

#sair imediatamente em caso de erro
set -e

java -cp "$BASEFOLDER/../../bftsmart/library/lib/*:"\
"$BASEFOLDER/../../bftsmart/bin/BFT-SMaRt.jar:"\
"$BASEFOLDER/../../protobuf/build/protobuf-runtime.jar:"\
"$BASEFOLDER/../protobuf/list2/build/lista-proto.jar:"\
"$BASEFOLDER/list2/build/bench_server.jar" bftsmart.diversity.demos.list.ServerLaunch $1 10000

