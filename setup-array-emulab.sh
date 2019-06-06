#!/bin/bash
set -b # reporta status de jobs imediatamente ao inves de so no fim do script

ssh -t -t -o "StrictHostKeyChecking no" caioysc@node0.expteste.freestore.emulab.net /bin/bash << 'EOF' 2>&1 | sed 's/^/SSH0:/' 
rm -rf bftsmart-diversity
git clone https://github.com/caioycosta/bftsmart-diversity.git --recursive
cd bftsmart-diversity
cd config
mv hostsEmulab.config hosts.config
cd ..
time make -j10
echo $? 
exit
EOF

echo "Finalizado."

