#!/usr/bin/python

import sys
import logging
from list_imp import *

# codigo main do programa

#sys.settrace(tracefunc)
#critical, error, warning, info, debug, notset
logging.basicConfig(stream=sys.stderr, level=logging.ERROR, format='%(asctime)s %(levelname)s %(message)s')
logging.info("teste")

#primeiro parametro. classpath. 
#segundo parametro. id replica
#terceiro para. caminh odll
bc = BFTList(sys.argv[1], sys.argv[2], sys.argv[3])
bc.finalizarJvm()
