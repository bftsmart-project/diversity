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
export PYTHONPATH=$FACDCLSPTH:$BASEFOLDER:$PROTOCLSPTH

echo 'Library path' $LD_LIBRARY_PATH
echo 'python path' $PYTHONPATH

cd $1

$BASEFOLDER/demolist.py "-Djava.class.path=$LIBCLSPTH/BFT-SMaRt.jar:$LIBCLSPTH/sl4j-api-1.5.8.jar:$LIBCLSPATH/slf4j-jdk14-1.5.8.jar:$LIBCLSPATH/netty-3.1.1.GA.jar:$LIBCLSPATH/commons-codec-1.5.jar:$DIVCLSPTH\bftsmart-diversity.jar" $2 $DLLCLSPTH/libbftsmr.so



