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

$BASEFOLDER/demolist.py "-Djava.class.path=$BASEFOLDER/$LIBCLSPTH/BFT-SMaRt.jar:"\
"$BASEFOLDER/$LIBCLSPTH/netty-all-4.0.25.Final.jar:"\
"$BASEFOLDER/$LIBCLSPTH/slf4j-jdk14-1.5.8.jar:"\
"$BASEFOLDER/$LIBCLSPTH/commons-codec-1.5.jar:"\
"$BASEFOLDER/$LIBCLSPTH/core-0.1.4.jar:"\
"$BASEFOLDER/$LIBCLSPTH/slf4j-api-1.5.8.jar:"\
"$BASEFOLDER/$DIVCLSPTH/bftsmart-diversity.jar" $2 \
$BASEFOLDER/$DLLCLSPTH/libbftsmr.so



