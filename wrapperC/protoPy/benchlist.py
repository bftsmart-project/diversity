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


INVOKORDFUNC = CFUNCTYPE(c_int, POINTER(c_char), c_int, POINTER(POINTER(c_char))) 
INVOKUNORDFUNC = CFUNCTYPE(c_int, POINTER(c_char), c_int, POINTER(POINTER(c_char)))
GETSNAPFUNC = CFUNCTYPE(c_int, POINTER(POINTER(c_char)))  
INSTALLSNAPFUNC = CFUNCTYPE(None, POINTER(c_char), c_int)
RELEASEFUNC = CFUNCTYPE(None, POINTER(c_char))


class BFTSMaRtServer(BFTJVM):
    def __init__(self,id):
        super(BFTSMaRtServer,self).__init__()     # chama o construtor da classe base

        def liberar(p):
            BFTJVM.libbft.bftsmartrelease(p)

        self.releaseCback = RELEASEFUNC(liberar)
        BFTJVM.libbft.implementReleaseExecuteOrderedBuffer(self.releaseCback)
        BFTJVM.libbft.implementReleaseExecuteUnorderedBuffer(self.releaseCback)
        BFTJVM.libbft.implementReleaseGetSnapshotBuffer(self.releaseCback)

        def intermediateInvokeOrdered(input, size, outmem):
            p = create_string_buffer(size)
            memmove(p, input, size)
            result = self.invokeOrdered(p)
            output = BFTJVM.libbft.bftsmartallocate(len(result))
            memmove(output, result, len(result))

            outmem[0] = cast(output, POINTER(c_char));

            return len(result)
    
        self.invokeOrdCback = INVOKORDFUNC(intermediateInvokeOrdered)
        BFTJVM.libbft.implementExecuteOrdered(self.invokeOrdCback)
        
        def intermediateInvokeUnordered(input, size, outmem):
                p = create_string_buffer(size)
                memmove(p, input, size)
                result = self.invokeUnordered(p)
                output = BFTJVM.libbft.bftsmartallocate(len(result))
                outmem[0] = cast(output, POINTER(c_char));
                memmove(output, result, len(result))
                return len(result) 

        self.invokeUnordCback = INVOKUNORDFUNC(intermediateInvokeUnordered)
        BFTJVM.libbft.implementExecuteUnordered(self.invokeUnordCback)

        def intermediateGetSnapshot(outmem):
                result = self.getSnapshot()
                output = BFTJVM.libbft.bftsmartallocate(len(result))
                outmem[0] = cast(output, POINTER(c_char));
                memmove(output, result, len(result))
                return len(result) 

        self.getSnapCback = GETSNAPFUNC(intermediateGetSnapshot)
        BFTJVM.libbft.implementgetSnapshot(self.getSnapCback)

        def intermediateInstallSnapshot(input, size):
                p = create_string_buffer(size)
                memmove(p, input, size)
                self.installSnapshot(p)

        self.installSnapCback = INSTALLSNAPFUNC(intermediateInstallSnapshot)
        BFTJVM.libbft.implementInstallSnapshot(self.installSnapCback)

        BFTJVM.libbft.startServiceReplica(int(id))

class BFTList(BFTSMaRtServer):
    def __init__(self,id):
        super(BFTList,self).__init__(id)      # chama o construtor da classe base
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


# codigo main do programa
bc = BFTList(sys.argv[1])
bc.finalizarJvm()



