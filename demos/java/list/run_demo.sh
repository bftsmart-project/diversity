#!/bin/bash

[ $# -eq 0 ] && { echo "Usage: $0 config_folder id_replica"; exit 1; }


#sair imediatamente em caso de erro
set -e

BASEFOLDER=`pwd`/`dirname $0`

LIBCLSPTH=../../../lib
DIVCLSPTH=../../../bft-smart/java/dist
FACDCLSPTH=../../../python
DLLCLSPTH=../../../bft-smart/intermediate/dist
PROTOCLSPTH=../protobuf/build

PASTAJAVA=$($BASEFOLDER/../../../../util/findjava.sh)

export LD_LIBRARY_PATH=$PASTAJAVA/jre/lib/amd64/server
export PYTHONPATH=$BASEFOLDER/$FACDCLSPTH:$BASEFOLDER:$BASEFOLDER/$PROTOCLSPTH:$BASEFOLDER/../../protobuf/build/lib/python2.7/site-packages

echo 'Library path' $LD_LIBRARY_PATH
echo 'python path' $PYTHONPATH

cd $1


echo
echo

java -cp "$BASEFOLDER/$LIBCLSPTH/*:"\
"$BASEFOLDER/../../protobuf/build/protobuf-runtime.jar:"\
"$BASEFOLDER/../protobuf/build/vazio-proto.jar:"\
"$BASEFOLDER/$DIVCLSPTH/bftsmart-diversity.jar:"\
"$BASEFOLDER/build/bench_server.jar" bftsmart.diversity.demos.zero.ServerLaunch $2 10000



