[ $# -eq 0 ] && { echo "Usage: $0 n_threads id_replica n_ops req_siz interval"; exit 1; }

BASEFOLDER=`pwd`/`dirname $0`

#the current directory , when running this script, must contain a config folder.

#sair imediatamente em caso de erro
set -e

java -cp "$BASEFOLDER/../../bftsmart/library/lib/*:"\
"$BASEFOLDER/../../bftsmart/bin/BFT-SMaRt.jar:"\
"$BASEFOLDER/bin/demo-java.jar" bftsmart.diversity.demos.zero.ZeroClient $1 $2 $3 $4 $5

