#!/usr/bin/python

import sys
import logging
import Vazio_pb2
from bftsmart_serverlib import *

class BFTList(BFTSMaRtServer):
    def __init__(self,classpth,id,dllpath):
        super(BFTList,self).__init__(classpth,id,dllpath)      # chama o construtor da classe base

    def execute(self, input):
        #pdb.set_trace()
        logging.info("execute")
        req = Vazio_pb2.Vazio()
        req.ParseFromString(input)
        rsp = Vazio_pb2.Vazio()
        return rsp.SerializeToString()

    def invokeOrdered(self, input):
        return self.execute(input)

    def invokeUnordered(self, input):
        return self.execute(input)

    def getSnapshot(self):
        est = Vazio_pb2.Vazio()
        return est.SerializeToString()

    def installSnapshot(self, input):
        est = Vazio_pb2.Vazio()
        est.ParseFromString(input)



