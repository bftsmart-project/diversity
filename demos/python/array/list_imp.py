#!/usr/bin/python

import sys
import logging
import Estado_pb2
import Request_pb2
import Response_pb2
import monotonic
from bftsmart_serverlib import *

class BFTList(BFTSMaRtServer):
    def __init__(self,classpth,id,dllpath,interval,respsize):
        super(BFTList,self).__init__(classpth,id,dllpath)
        self.respsize = respsize
        self.rqst_count = 0
        self.interval = interval
        self.start_time = 0

    def execute(self, input):
        if self.start_time == 0:
            self.start_time = monotonic.monotonic()
        logging.info("execute")
        req = Request_pb2.Request()
        req.ParseFromString(input)
        rsp = Response_pb2.Response()
	rsp.Data = bytearray(self.respsize)
        self.rqst_count++;
        if self.rqst_count >= self.interval:
            now = monotonic.monotonic()
            print "Throughput: {0} /s".format(self.rqst_count / now - self.start_time)
            self.rqst_count = 0
            self.start_time = monotonic.monotonic()
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



