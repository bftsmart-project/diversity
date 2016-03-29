#!/usr/bin/python

from ctypes import *
#import pdb
import sys
import logging
import Vazio_pb2

def tracefunc(frame, event, arg, indent=[0]):
      if event == "call":
          indent[0] += 2
          print "-" * indent[0] + "> call function", frame.f_code.co_name
      elif event == "return":
          print "<" + "-" * indent[0], "exit function", frame.f_code.co_name
          indent[0] -= 2
      return tracefunc


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
            # especifica retorno das funcoes C que nao retornam int
            BFTJVM.libbft.setClasspath.restype = None
            BFTJVM.libbft.bftsmartallocate.restype = c_void_p
            BFTJVM.libbft.bftsmartrelease.restype = None
            BFTJVM.libbft.finalizarJvm.restype = None
            BFTJVM.libbft.implementExecuteOrdered.restype = None
            BFTJVM.libbft.implementExecuteUnordered.restype = None
            BFTJVM.libbft.implementInstallSnapshot.restype = None
            BFTJVM.libbft.implementfunctionCallTest.restype = None
            BFTJVM.libbft.implementgetSnapshot.restype = None
            BFTJVM.libbft.implementReleaseGetSnapshotBuffer.restype = None
            BFTJVM.libbft.implementReleaseExecuteOrderedBuffer.restype = None
            BFTJVM.libbft.implementReleaseExecuteUnorderedBuffer.restype = None


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
CALLTESTFUNC = CFUNCTYPE(None)


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
            logging.info("io.")
            p = create_string_buffer(size)
            memmove(p, input, size)
            result = self.invokeOrdered(p)
            output = BFTJVM.libbft.bftsmartallocate(len(result))
            memmove(output, result, len(result))

            outmem[0] = cast(output, POINTER(c_char));
            logging.info("fimio.")
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
                logging.info("in get snap");
                result = self.getSnapshot()
                output = BFTJVM.libbft.bftsmartallocate(len(result))
                outmem[0] = cast(output, POINTER(c_char));
                memmove(output, result, len(result))
                logging.info("out get snap");
                return len(result) 

        self.getSnapCback = GETSNAPFUNC(intermediateGetSnapshot)
        BFTJVM.libbft.implementgetSnapshot(self.getSnapCback)

        def intermediateInstallSnapshot(input, size):
                logging.info("in instal snap");
                p = create_string_buffer(size)
                memmove(p, input, size)
                self.installSnapshot(p)
                logging.info("out install snap");

        self.installSnapCback = INSTALLSNAPFUNC(intermediateInstallSnapshot)
        BFTJVM.libbft.implementInstallSnapshot(self.installSnapCback)

        def functionCallTest():
            logging.info("function call test success.")
    
        self.funccalltestCback = CALLTESTFUNC(functionCallTest)
        BFTJVM.libbft.implementfunctionCallTest(self.funccalltestCback)

        BFTJVM.libbft.startServiceReplica(int(id))

class BFTList(BFTSMaRtServer):
    def __init__(self,id):
        super(BFTList,self).__init__(id)      # chama o construtor da classe base

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


# codigo main do programa

#sys.settrace(tracefunc)
#critical, error, warning, info, debug, notset
logging.basicConfig(stream=sys.stderr, level=logging.ERROR, format='%(asctime)s %(levelname)s %(message)s')
logging.info("teste")

bc = BFTList(sys.argv[1])
bc.finalizarJvm()



