#!/usr/bin/python

import sys
import logging
from bftsmart_serverlib import *

class BFTList(BFTSMaRtServer):
    def __init__(self,classpth,id,dllpath, resp_size):
        super(BFTList,self).__init__(classpth,id,dllpath)      # chama o construtor da classe base
        self.resp_size = resp_size

    def execute(self, input):
        #pdb.set_trace()
        logging.info("execute")
        resp = '\xFF' * self.resp_size
        return resp 

    def invokeOrdered(self, input):
        return self.execute(input)

    def invokeUnordered(self, input):
        return self.execute(input)

    def getSnapshot(self):
        return []

    def installSnapshot(self, input):
        pass


