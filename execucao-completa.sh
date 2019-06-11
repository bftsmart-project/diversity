#!/bin/bash
set -b # reporta status de jobs imediatamente ao inves de so no fim do script

for i in python java c cpp go	
do
for j in 0 512 1024 4096
do
echo
date
echo Caso $i $j
./execucao-array-emulab.sh 100 2000 $j 5000 $i | grep -E '(LATENCY|STNDDEV|MAXTHRO)'
echo
done
done

echo "Finalizado."

