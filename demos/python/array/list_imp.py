#!/usr/bin/python

import sys
import logging
import Estado_pb2
import Request_pb2
import Response_pb2
from bftsmart_serverlib import *

class BFTList(BFTSMaRtServer):
    def __init__(self,classpth,id,dllpath,respsize):
        super(BFTList,self).__init__(classpth,id,dllpath)
        self.respsize = respsize

    def execute(self, input):
        logging.info("execute")
        req = Request_pb2.Request()
        req.ParseFromString(input)
        rsp = Response_pb2.Response()
	rsp.Data = bytearray(self.respsize)
        return rsp.SerializeToString()

    def invokeOrdered(self, input):
        return self.execute(input)

    def invokeUnordered(self, input):
        return self.execute(input)

    def getSnapshot(self):
        est = Estado_pb2.Estado()
        return est.SerializeToString()

    def installSnapshot(self, input):
        est = Estado_pb2.Estado()
        est.ParseFromString(input)



