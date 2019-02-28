#!/bin/bash
set -b # reporta status de jobs imediatamente ao inves de so no fim do script

ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH1:/' &
cd worker/compilar-projeto/build
./demos/go/run_zero-plain_server.sh 0 $1 1000 
EOF
JOB1=$! 

sleep 10

ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH2:/' &
cd worker/compilar-projeto/build
./demos/go/run_zero-plain_server.sh 1 $1 1000
EOF
JOB2=$! 

sleep 10

ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH3:/' &
cd worker/compilar-projeto/build
./demos/go/run_zero-plain_server.sh 2 $1 1000
EOF
JOB3=$! 

sleep 10

ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH4:/' &
cd worker/compilar-projeto/build
./demos/go/run_zero-plain_server.sh 3 $1 1000
EOF
JOB4=$! 

sleep 20

ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH5:/' &
cd worker/compilar-projeto/build
./demos/java/run_zero-plain_client.sh 5 7001 50 $1 1000
EOF
JOB5=$! 

#top -b -d 7 &
echo Sleeping.
sleep 90 
#kill %1
kill $JOB1
kill $JOB2
kill $JOB3
kill $JOB4
kill $JOB5
echo "Finalizado."

