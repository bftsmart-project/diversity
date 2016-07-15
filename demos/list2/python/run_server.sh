#!/bin/bash

[ $# -eq 0 ] && { echo "Usage: $0 config_folder id_replica"; exit 1; }


#sair imediatamente em caso de erro
set -e

BASEFOLDER=`pwd`/`dirname $0`

#caminhos relativos a seguir sempre em relacao a basefolder

# bibliotecas (bft smart, netty etc)
LIBCLSPTH=../../../lib
# intermediarios python
FACDCLSPTH=../../../diversity/python
# dll libbftsmr.so
DLLCLSPTH=../../../diversity/c/build/libbftsmr.so
# mensagens compiladas
PROTOCLSPTH=../protobuf/build

#pasta onde o java esta instalado no sistema.
PASTAJAVA=$(../../../scripts/findjava.sh)

export LD_LIBRARY_PATH=$PASTAJAVA/jre/lib/amd64/server
export PYTHONPATH=$BASEFOLDER/$FACDCLSPTH:$BASEFOLDER:$BASEFOLDER/$PROTOCLSPTH:$BASEFOLDER/../../../protobuf/build/lib/python2.7/site-packages

echo 'Library path' $LD_LIBRARY_PATH
echo 'python path' $PYTHONPATH

cd $1

echo
echo

$BASEFOLDER/demolist.py "-Djava.class.path=$BASEFOLDER/$LIBCLSPTH/BFT-SMaRt.jar:"\
"$BASEFOLDER/$LIBCLSPTH/slf4j-api-1.5.8.jar:"\
"$BASEFOLDER/$LIBCLSPTH/slf4j-jdk14-1.5.8.jar:"\
"$BASEFOLDER/$LIBCLSPTH/netty-all-4.0.25.Final.jar:"\
"$BASEFOLDER/$LIBCLSPTH/commons-codec-1.5.jar:"\
"$BASEFOLDER/$LIBCLSPTH/core-0.1.4.jar:"\
"$BASEFOLDER/../../../diversity/java/build/bftsmart-diversity.jar" $2 $BASEFOLDER/$DLLCLSPTH


