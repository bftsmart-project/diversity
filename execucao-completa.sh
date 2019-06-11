#!/bin/bash
set -b # reporta status de jobs imediatamente ao inves de so no fim do script

for i in go c cpp python java	
do
for j in 0 512 1024 4096
do
for k in 0 1 2 3 4
do
ssh -t -t -o "StrictHostKeyChecking no" caioysc@node$k.expteste.freestore.emulab.net /bin/bash << EOF 2>&1 
killall -KILL /usr/bin/python
killall -KILL bench_server.out
killall -KILL java
killall -KILL go-server.exe
exit
EOF
done
echo
date
echo Caso $i $j
./execucao-array-emulab.sh 100 2000 $j 5000 $i | grep -E '(LATENCY|STNDDEV|MAXTHRO)'
echo
done
done

echo "Finalizado."

