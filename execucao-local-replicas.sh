#!/bin/bash
set -b # reporta status de jobs imediatamente ao inves de so no fim do script

ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH1:/' &
cd worker/compilar-projeto/build
./demos/go/run_zero-plain_server.sh 
EOF
JOB1=$! 


ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH2:/' &
echo Teste
>&2 echo Teste2ERR 
echo Teste3
date
sleep 6 
date
echo quitting
EOF
JOB2=$! 


ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH3:/' &
echo Teste
>&2 echo Teste2ERR 
echo Teste3
date
sleep 7 
date
echo quitting
EOF
JOB3=$! 


ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH4:/' &
echo Teste
>&2 echo Teste2ERR 
echo Teste3
date
sleep 8 
date
echo quitting
EOF
JOB4=$! 


ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH5:/' &
echo Teste
>&2 echo Teste2ERR 
echo Teste3
date
sleep 9 
date
echo quitting
EOF
JOB5=$! 

#top -b -d 7 &
echo Sleeping.
sleep 15 
#kill %1
kill $JOB1
kill $JOB2
kill $JOB3
kill $JOB4
kill $JOB5
echo "Finalizado."

