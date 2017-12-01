#!/bin/bash

[ $# -eq 0 ] && { echo "Usage: $0 config_folder id_replica"; exit 1; }


#sair imediatamente em caso de erro
set -e

BASEFOLDER=`pwd`/`dirname $0`

LIBCLSPTH=../../bftsmart/library/lib
DIVCLSPTH=../../diversity/java/build
PROTOCLSPTH=../protobuf/list2/build

PASTAJAVA=$($BASEFOLDER/../../scripts/findjava.sh)

export LD_LIBRARY_PATH=$PASTAJAVA/jre/lib/amd64/server

echo 'Library path' $LD_LIBRARY_PATH


echo
echo

$BASEFOLDER/list2/build/go-server.exe $1 "-Djava.class.path=$BASEFOLDER/../../bftsmart/bin/BFT-SMaRt.jar:"\
"$BASEFOLDER/$LIBCLSPTH/slf4j-api-1.5.8.jar:"\
"$BASEFOLDER/$LIBCLSPTH/slf4j-jdk14-1.5.8.jar:"\
"$BASEFOLDER/$LIBCLSPTH/netty-all-4.1.9.Final.jar:"\
"$BASEFOLDER/$LIBCLSPTH/commons-codec-1.5.jar:"\
"$BASEFOLDER/$LIBCLSPTH/core-0.1.4.jar:"\
"$BASEFOLDER/$DIVCLSPTH/bftsmart-diversity.jar"



