#!/usr/bin/python

from ctypes import *
import sys
import testcount_pb2


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
        p = create_string_buffer(strobj)        # e retornando uma string binaria tambem.
        r = create_string_buffer(100)
        resultado = BFTJVM.libbft.invokeOrdered(p, sizeof(p), r)
        x = create_string_buffer(r.raw, resultado)
        return r.raw

    def invokeUnordered(self, strobj):              # invoca o unordered
                p = create_string_buffer(strobj)          
                r = create_string_buffer(100) 
                resultado = BFTJVM.libbft.invokeUnordered(p.raw, sizeof(p), r) 
                x = create_string_buffer(r.raw, resultado) 
                return r.raw

INVOKORDFUNC = CFUNCTYPE(c_int, POINTER(c_char), c_int, POINTER(c_char)) 
INVOKUNORDFUNC = CFUNCTYPE(c_int, POINTER(c_char), c_int, POINTER(c_char)) 
GETSNAPFUNC = CFUNCTYPE(c_int, POINTER(c_char))  
INSTALLSNAPFUNC = CFUNCTYPE(None, POINTER(c_char), c_int)


class BFTSMaRtServer(BFTJVM):
    def __init__(self,id):
        super(BFTSMaRtServer,self).__init__()     # chama o construtor da classe base
        BFTJVM.libbft.startServiceReplica(int(id))
        def intermediateInvokeOrdered(input, size, output):
            p = create_string_buffer(size)
            memmove(p, input, size)
            result = self.invokeOrdered(p)
            memmove(output, result, len(result))
            return len(result)
    
        self.invokeOrdCback = INVOKORDFUNC(intermediateInvokeOrdered)
        BFTJVM.libbft.implementExecuteOrdered(self.invokeOrdCback)
        
        def intermediateInvokeUnordered(input, size, output):
                p = create_string_buffer(size)
                memmove(p, input, size)
                result = self.invokeUnordered(p)
                memmove(output, result, len(result))
                return len(result) 

        self.invokeUnordCback = INVOKUNORDFUNC(intermediateInvokeUnordered)
        BFTJVM.libbft.implementExecuteUnordered(self.invokeUnordCback)

        def intermediateGetSnapshot(output):
                result = self.getSnapshot()
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


class ImplUsuario(BFTSMaRtServer):
    def __init__(self,id):
        super(ImplUsuario,self).__init__(id)      # chama o construtor da classe base
        self.state = testcount_pb2.Counter()
        self.state.value = 0

    def invokeOrdered(self, input):
        self.state.value = self.state.value + 1;
        return self.state.SerializeToString()

    def invokeUnordered(self, input):
        return self.state.SerializeToString()

    def getSnapshot(self):
        return self.state.SerializeToString()

    def installSnapshot(self, input):
        self.state.ParseFromString()


# codigo main do programa
bc = ImplUsuario(sys.argv[1])
bc.finalizarJvm()



