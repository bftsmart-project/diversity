#!/usr/bin/python

from ctypes import *
from bftsmart_clientlib import *
import Estado_pb2
import Request_pb2
import Response_pb2

# codigo main do programa
bc = BFTSMaRtClient(sys.argv[1], sys.argv[2], sys.argv[3])


mydata = raw_input('acao(add,remove,size,contains,get,sair) :')
while mydata != 'sair':
    if mydata == 'add':
        req = Request_pb2.Request()
        req.action = Request_pb2.Request.ADD
        req.value = int(raw_input('numero:'))        
        rsp = bc.invokeOrdered(req.SerializeToString())
        rspP = Response_pb2.Response()
        rspP.ParseFromString(rsp)
        print 'resposta:'
        print rspP.BoolResponse
    if mydata == 'remove':
        req = Request_pb2.Request()
        req.action = Request_pb2.Request.REMOVE
        req.value = int(raw_input('numero:'))
        rsp = bc.invokeOrdered(req.SerializeToString())
        rspP = Response_pb2.Response()
        rspP.ParseFromString(rsp)
        print 'resposta:'
        print rspP.BoolResponse
    if mydata == 'size':
        req = Request_pb2.Request()
        req.action = Request_pb2.Request.SIZE
        ser = req.SerializeToString()

        
        print 'tamanho: '
        print len(ser)
        print ':'.join(x.encode('hex') for x in ser)
        req.ParseFromString(ser) #testa o parse

        rsp = bc.invokeOrdered(ser)

        rspP = Response_pb2.Response()
        rspP.ParseFromString(rsp)
        print 'resposta:'
        print rspP.IntResponse
    if mydata == 'contains':
        req = Request_pb2.Request()
        req.action = Request_pb2.Request.CONTAINS
        req.value = int(raw_input('numero:'))
        rsp = bc.invokeOrdered(req.SerializeToString())
        rspP = Response_pb2.Response()
        rspP.ParseFromString(rsp)
        print 'resposta:'
        print rspP.BoolResponse
    if mydata == 'get':
        req = Request_pb2.Request()
        req.action = Request_pb2.Request.GET
        req.value = int(raw_input('numero:'))
        rsp = bc.invokeOrdered(req.SerializeToString())
        rspP = Response_pb2.Response()

        print 'py-resposta-tamanho: '
        print len(rsp)
        print ':'.join(x.encode('hex') for x in rsp)


        rspP.ParseFromString(rsp)
        print 'resposta:'
        print rspP.IntResponse
    mydata = raw_input('acao(add,remove,size,contains,get,sair) :')
print 'pressione ctrl-c.'
bc.finalizarJvm()



