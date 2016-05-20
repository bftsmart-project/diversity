#!/usr/bin/python

from ctypes import *
import sys
from BFTJVM import *

class BFTSMaRtClient(BFTJVM):
    def __init__(self, clspath, id, dllpath):
        super(BFTSMaRtClient,self).__init__(dllpath, clspath)     # chama o construtor da classe base
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

# exemplo de instanciacao. 
# bc = BFTSMaRtClient(sys.argv[1])
# rsp = bc.invokeOrdered(req.SerializeToString())
# bc.finalizarJvm()



