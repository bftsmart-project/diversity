#!/usr/bin/python

from ctypes import *
import sys
from KVReplica import *


# codigo main do programa
bc = BFTKV(sys.argv[1],sys.argv[2],sys.argv[3])
bc.finalizarJvm()



