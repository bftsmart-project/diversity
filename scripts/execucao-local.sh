#!/bin/bash
set -b

ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF 2>&1 | sed 's/^/SSH1:/' &
set -e
cd worker
cd compilar-projeto
cd build
EOF
JOB1=$! 

#top -b -d 7 &
echo Sleeping.
sleep 7 
#kill %1
kill $JOB1

