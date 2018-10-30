#!/bin/bash
set -b

ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH1:/' &
echo Teste
>&2 echo Teste2ERR 
echo Teste3
sleep 10
echo quitting
EOF
JOB1=$! 

#top -b -d 7 &
echo Sleeping.
sleep 7 
#kill %1
kill $JOB1

