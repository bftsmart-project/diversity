#!/usr/bin/python

from ctypes import *
#import pdb
import sys
import logging
import Estado_pb2
import Request_pb2
import Response_pb2

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

class Elemento(object):
    valor = 0
    proximo = None;
    
class ListaNormal(object):
    inicio = None
    fim = None
  
    def insereInicio(self, valor):
        novoprimeiro = Elemento()
        novoprimeiro.valor = valor
        novoprimeiro.proximo = self.inicio
        self.inicio = novoprimeiro
        if self.fim is None:
            self.fim = self.inicio
        return 0
        
    def insereFinal(self, valor):
        novoultimo = Elemento()
        novoultimo.valor = valor
        novoultimo.proximo = None
        if self.inicio is None:
            self.inicio = novoultimo
        else:
            self.fim.proximo = novoultimo
        self.fim = novoultimo
        return 0
        
    def estaVazia(self):
        if self.inicio is None:
            return 1
        return 0
        
    def removeInicio(self):
        if self.inicio is None:
            return -1
        tmp = self.inicio.valor
        removido = self.inicio
        self.inicio = self.inicio.proximo
        if self.inicio is None:
          self.fim = None
        return tmp
        
    def removeFinal(self):
        if self.inicio is None:
            return -1
        if self.inicio is self.fim:
            tmp = self.inicio.valor
            self.inicio = None
            self.fim = None
            return tmp
        penultimo = self.inicio
        while penultimo.proximo is not self.fim:
            penultimo = penultimo.proximo
        tmp = penultimo.proximo.valor
        self.fim = penultimo
        penultimo.proximo = None
        return tmp
        
    def remover(self, pos):
        if self.inicio is None:
            return -1
        if pos < 0:
            return -1
        removido = self.inicio
        ant_removido = None
        for x in xrange(0,pos):
            if removido is None:
                break
            ant_removido = removido
            removido = removido.proximo
        if removido is not None:
            if removido is self.inicio:
                self.inicio = removido.proximo
            else:
                ant_removido.proximo = removido.proximo
            if removido is self.fim:
                self.fim = ant_removido
                
            tmp = removido.valor
        
            return tmp;
        else:
            return -1    
        
    def buscarValor(self, pos):
        atual = self.inicio
        i = 0
        while atual is not None:
            if i == pos:
                return atual.valor
            i += 1
            atual = atual.proximo
        return -1
        
    def buscarIndice(self, valor):
        atual = self.inicio
        i = 0
        while atual is not None:
            if atual.valor == valor:
                return i
            i += 1
            atual = atual.proximo
        return -1
        
    def tamanhoLista(self):
        atual = self.inicio
        i = 0
        while atual is not None:
            i += 1
            atual = atual.proximo
        return i
        
    def inserir(self, pos, valor):
        if pos <= 0:
            self.insereInicio(valor)
        else:
            atual = self.inicio
            for x in xrange(0,pos-2):
                if atual is not None:
                    break
                atual = atual.proximo
            if atual is None or atual is self.fim:
                insereFinal(valor)
            else:
                novo = Elemento()
                novo.valor = valor
                novo.proximo = atual.proximo
                atual.proximo = novo
        return 0
        
class BFTList(BFTSMaRtServer):
    def __init__(self,id):
        super(BFTList,self).__init__(id)      # chama o construtor da classe base
        self.state = ListaNormal()        

    def execute(self, input):
        #pdb.set_trace()
        logging.info("execute")
        req = Request_pb2.Request()
        req.ParseFromString(input)
        logging.debug("action: " + str(req.action))
        logging.debug("value: " + str(req.value))
        if req.action == Request_pb2.Request.ADD:
            val = req.value;
            x = self.state.buscarIndice(val)
            rsp = Response_pb2.Response()
            rsp.BoolResponse = False
            if x < 0:
              rsp.BoolResponse = True
              self.state.insereFinal(val)
            
            logging.debug("rsp:" + str(rsp.BoolResponse))
            return rsp.SerializeToString()

        if req.action == Request_pb2.Request.REMOVE:
            val = req.value
            res = True
            
            y = self.state.buscarIndice(val)
            logging.debug("buscaIndice:" + str(y))
            if y < 0:
              res = False
            else:
              self.state.remover(y)
            
            rsp = Response_pb2.Response()
            rsp.BoolResponse = res
            logging.debug("rsp:" + str(res))
            return rsp.SerializeToString()

        if req.action == Request_pb2.Request.SIZE:            
            rsp = Response_pb2.Response()
            rsp.IntResponse = self.state.tamanhoLista()
            return rsp.SerializeToString()

        if req.action == Request_pb2.Request.CONTAINS: 
            val = req.value
            res = False
            if self.state.buscarIndice(val) >= 0:
                res = True;
            rsp = Response_pb2.Response()
            rsp.BoolResponse = res
            return rsp.SerializeToString()

        if req.action == Request_pb2.Request.GET:
            val = req.value
            res = -1
            
            rsp = Response_pb2.Response()
            rsp.IntResponse = self.state.buscarValor(val)
            return rsp.SerializeToString()

    def invokeOrdered(self, input):
        return self.execute(input)

    def invokeUnordered(self, input):
        return self.execute(input)

    def getSnapshot(self):
        est = Estado_pb2.Estado()
        atual = self.state.inicio
        while atual is not None:
            est.lista.append(atual.valor)
            atual = atual.proximo
        return est.SerializeToString()

    def installSnapshot(self, input):
        est = Estado_pb2.Estado()
        est.ParseFromString(input)
        self.state = ListaNormal()
        for i in est.lista:
            self.state.insereFinal(i)


# codigo main do programa

#sys.settrace(tracefunc)
#critical, error, warning, info, debug, notset
logging.basicConfig(stream=sys.stderr, level=logging.ERROR, format='%(asctime)s %(levelname)s %(message)s')
logging.info("teste")

bc = BFTList(sys.argv[1])
bc.finalizarJvm()



