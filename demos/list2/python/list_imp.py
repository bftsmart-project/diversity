#!/usr/bin/python

from ctypes import *
import sys
import Estado_pb2
import Request_pb2
import Response_pb2
from bftsmart_serverlib import *


class BFTList(BFTSMaRtServer):
    def __init__(self,classpth,id,dllpath):
        super(BFTList,self).__init__(classpth,id,dllpath)
        self.state = list()        

    def execute(self, input):
        req = Request_pb2.Request()
        req.ParseFromString(input)

        if req.action == Request_pb2.Request.ADD:
            val = req.value;
            res = False;
            if val not in self.state:
                self.state.append(val)
                res = True;
            rsp = Response_pb2.Response()
            rsp.BoolResponse = res
            return rsp.SerializeToString()

        if req.action == Request_pb2.Request.REMOVE:
            val = req.value;
            res = False;
            if val in self.state:
                del self.state[self.state.index(val)]
                res = True;

            rsp = Response_pb2.Response()
            rsp.BoolResponse = res
            return rsp.SerializeToString()

        if req.action == Request_pb2.Request.SIZE:            
            rsp = Response_pb2.Response()
            rsp.IntResponse = len(self.state)
            return rsp.SerializeToString()

        if req.action == Request_pb2.Request.CONTAINS: 
            val = req.value;
            res = False;
            if val in self.state:
                res = True;
            rsp = Response_pb2.Response()
            rsp.BoolResponse = res
            return rsp.SerializeToString()

        if req.action == Request_pb2.Request.GET:
            val = req.value;
            res = -1;
            if val < len(self.state):
                res = self.state[val]
            rsp = Response_pb2.Response()
            rsp.IntResponse = res
            return rsp.SerializeToString()

    def invokeOrdered(self, input):
        return self.execute(input)

    def invokeUnordered(self, input):
        return self.execute(input)

    def getSnapshot(self):
        est = Estado_pb2.Estado()
        for i in self.state:
            est.lista.append(i)
        return est.SerializeToString()

    def installSnapshot(self, input):
        est = Estado_pb2.Estado()
        est.ParseFromString(input)
        self.state = list()
        for i in est.lista:
            self.state.append(i)
