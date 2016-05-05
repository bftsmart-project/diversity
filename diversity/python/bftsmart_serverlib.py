#!/usr/bin/python

from ctypes import *
import sys
import logging
from BFTJVM import *

class BFTSMaRtServer(BFTJVM):
    def __init__(self,clspath,id,dllpath):
        super(BFTSMaRtServer,self).__init__(dllpath,clspath)     # chama o construtor da classe base

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




