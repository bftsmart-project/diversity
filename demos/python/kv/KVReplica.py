#!/usr/bin/python

from ctypes import *
import sys
import Estado_pb2
import Request_pb2
import Response_pb2
from bftsmart_serverlib import *


class BFTKV(BFTSMaRtServer):
    def __init__(self,classpth,id,dllpath):
		self.state = {}
		super(BFTKV,self).__init__(classpth,id,dllpath)

    def execute(self, input):
        req = Request_pb2.Request()
        req.ParseFromString(input)

        if req.action == Request_pb2.Request.PUT:
            self.state[req.key] = req.value            
            res = True;
            rsp = Response_pb2.Response()
            rsp.BoolResponse = res
            return rsp.SerializeToString()

        if req.action == Request_pb2.Request.DELETE:
            res = False
            if req.key in self.state:
                del self.state[req.key]           
                res = True
            rsp = Response_pb2.Response()
            rsp.BoolResponse = res
            return rsp.SerializeToString()

        if req.action == Request_pb2.Request.LIST:            
            rsp = Response_pb2.Response()
            keys = list(self.state.keys())
            keys.sort()
            for i in keys:
                rsp.ListResponse.append(i)
            rsp.BoolResponse = True
            return rsp.SerializeToString()

        if req.action == Request_pb2.Request.GET:           
            res = False;
            rsp = Response_pb2.Response()
            if req.key in self.state:
                res = True
                rsp.StringResponse = self.state[req.key]
            rsp.BoolResponse = res
            return rsp.SerializeToString()

    def invokeOrdered(self, input):
        return self.execute(input)

    def invokeUnordered(self, input):
        return self.execute(input)

    def getSnapshot(self):
        est = Estado_pb2.MapFieldEntry()
        keys = list(self.state.items())
        keys.sort()
        for k in keys:
            pair = Estado_pb2.MapFieldEntry()
            pair.key = k
            pair.value = self.state[k]
            est.kv.append(pair)            
        return est.SerializeToString()

    def installSnapshot(self, input):
        est = Estado_pb2.Estado()
        est.ParseFromString(input)
        self.state = {}
        for i in est.kv:
            self.state[i.key] = i.value
