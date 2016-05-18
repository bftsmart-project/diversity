#!/usr/bin/python

import sys
import logging
import Estado_pb2
import Request_pb2
import Response_pb2
from bftsmart_serverlib import *

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





