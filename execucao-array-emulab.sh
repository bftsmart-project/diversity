#!/bin/bash
set -b # reporta status de jobs imediatamente ao inves de so no fim do script

ssh -t -t -o "StrictHostKeyChecking no" caioysc@node1.expteste.freestore.emulab.net /bin/bash << EOF 2>&1 | sed 's/^/SSH1:/' &
cd bftsmart-diversity
./demos/$5/run_array_server.sh 0 $3 $4
EOF
JOB2=$! 

sleep 1

ssh -t -t -o "StrictHostKeyChecking no" caioysc@node2.expteste.freestore.emulab.net /bin/bash << EOF 2>&1 | sed 's/^/SSH2:/' &
cd bftsmart-diversity
./demos/$5/run_array_server.sh 1 $3 $4
EOF
JOB3=$! 

sleep 1

ssh -t -t -o "StrictHostKeyChecking no" caioysc@node3.expteste.freestore.emulab.net /bin/bash << EOF 2>&1 | sed 's/^/SSH3:/' &
cd bftsmart-diversity
./demos/$5/run_array_server.sh 2 $3 $4
EOF
JOB4=$! 

sleep 1

ssh -t -t -o "StrictHostKeyChecking no" caioysc@node4.expteste.freestore.emulab.net /bin/bash << EOF 2>&1 | sed 's/^/SSH4:/' &
cd bftsmart-diversity
./demos/$5/run_array_server.sh 3 $3 $4
EOF
JOB5=$! 

sleep 30

ssh -t -t -o "StrictHostKeyChecking no" caioysc@node0.expteste.freestore.emulab.net /bin/bash << EOF 2>&1 | sed 's/^/SSH0:/' &
cd bftsmart-diversity
./demos/java/run_array_client.sh 0 $1 $2 $3 
exit
EOF
JOB1=$! 

sleep 1
#top -b -d 7 &
echo Sleeping.
sleep 20 
#kill %1
kill $JOB1
kill $JOB2
kill $JOB3
kill $JOB4
kill $JOB5
echo "Finalizado."

