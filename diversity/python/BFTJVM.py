#!/usr/bin/python

from ctypes import *
import sys
import logging

def tracefunc(frame, event, arg, indent=[0]):
      if event == "call":
          indent[0] += 2
          print "-" * indent[0] + "> call function", frame.f_code.co_name
      elif event == "return":
          print "<" + "-" * indent[0], "exit function", frame.f_code.co_name
          indent[0] -= 2
      return tracefunc

class BFTJVM(object):
    jvmCarregada = False                # flag para controle do carregamento da Jvm

    def __init__(self, dllpath, classpth):             # construtor
        BFTJVM.libbft = CDLL(dllpath);
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

            BFTJVM.libbft.setClasspath(c_char_p(classpth));
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

