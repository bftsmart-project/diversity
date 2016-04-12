#!/usr/bin/python

from ctypes import *
import sys
import Estado_pb2
import Request_pb2
import Response_pb2


# import addressbook_pb2
# person = addressbook_pb2.Person()
# person.id = 1234
# person.name = "John Doe"
# person.email = "jdoe@example.com"
# phone = person.phone.add()
# phone.number = "555-4321"
# phone.type = addressbook_pb2.Person.HOME

# SerializeToString(): serializes the message and returns it as a string. Note that the bytes are binary, 
# not text; we only use the str type as a convenient container. 
# ParseFromString(data): parses a message from the given string. 

# address_book = addressbook_pb2.AddressBook()

# # Read the existing address book.
# f = open(sys.argv[1], "rb")
# address_book.ParseFromString(f.read())

class BFTJVM(object):
    libbft = CDLL("libbftsmr.so")           # carrega a DLL do wrapper, 1 vez somente por execucao
    jvmCarregada = False                # flag para controle do carregamento da Jvm

    def __init__(self):             # construtor
        if BFTJVM.jvmCarregada == False:
            BFTJVM.libbft.carregarJvm()     # carrega a Jvm, apenas uma vez
            BFTJVM.jvmCarregada = True

    def finalizarJvm(self):
        if BFTJVM.jvmCarregada == True:
            BFTJVM.libbft.finalizarJvm()
            BFTJVM.jvmCarregada = False
                    

class BFTSMaRtClient(BFTJVM):
    def __init__(self, id):
        super(BFTSMaRtClient,self).__init__()     # chama o construtor da classe base
        BFTJVM.libbft.createServiceProxy(int(id)) # cria o client wrapper com um id tipo (int)
    
    def invokeOrdered(self, strobj):        # invoca o ordered, recebendo uma string binaria
        p = create_string_buffer(strobj, len(strobj))        # e retornando uma string binaria tambem.
        r = create_string_buffer(100)
        resultado = BFTJVM.libbft.invokeOrdered(p, len(p.raw), r)

        #apenas para debug
        print ' tamanho:'
        print resultado
        print ':'.join(x.encode('hex') for x in r.raw)
        #fim debug

        x = create_string_buffer(r.raw[:resultado], resultado)
        return x.raw

    def invokeUnordered(self, strobj):              # invoca o unordered
        p = create_string_buffer(strobj, len(strobj))          
        r = create_string_buffer(100) 
        resultado = BFTJVM.libbft.invokeUnordered(p, len(p.raw), r) 
        x = create_string_buffer(r.raw[:resultado], resultado) 
        return x.raw

# codigo main do programa
bc = BFTSMaRtClient(sys.argv[1])


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



