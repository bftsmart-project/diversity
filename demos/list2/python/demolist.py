#!/usr/bin/python

from ctypes import *
import sys
from list_imp import *


# codigo main do programa
bc = BFTList(sys.argv[1],sys.argv[2],sys.argv[3])
bc.finalizarJvm()



