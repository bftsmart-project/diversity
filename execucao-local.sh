#!/bin/bash
ssh -o "StrictHostKeyChecking no" localhost /bin/bash << EOF | sed 's/^/SSH1:/' &
echo Teste
echo Teste2 
echo Teste3
EOF
#top -b -d 7 &
echo Sleeping.
sleep 30
#kill %1

