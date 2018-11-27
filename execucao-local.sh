#!/bin/bash
set -b # reporta status de jobs imediatamente ao inves de so no fim do script

ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH1:/' &
echo Teste
>&2 echo Teste2ERR 
echo Teste3
sleep 5
echo quitting
EOF
JOB1=$! 


ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH2:/' &
echo Teste
>&2 echo Teste2ERR 
echo Teste3
sleep 6 
echo quitting
EOF
JOB2=$! 


ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH3:/' &
echo Teste
>&2 echo Teste2ERR 
echo Teste3
sleep 7 
echo quitting
EOF
JOB3=$! 


ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH4:/' &
echo Teste
>&2 echo Teste2ERR 
echo Teste3
sleep 8 
echo quitting
EOF
JOB4=$! 


ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH5:/' &
echo Teste
>&2 echo Teste2ERR 
echo Teste3
sleep 9 
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

