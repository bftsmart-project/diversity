package bftsmart

// #include "init-go.h"
// #include <bftsmart-c-client.h>
import "C"
import "unsafe"

func Init() {
C.init_go()
}

type ExecuteFunc func([]byte)[]byte
type GetSnapFunc func()[]byte
type InstallSnapFunc func([]byte)
type FunctionCallTestFunc func()

var ExecuteOrderedImplementation ExecuteFunc
var ExecuteUnorderedImplementation ExecuteFunc
var GetSnapImplementation GetSnapFunc
var InstallSnapImplementation InstallSnapFunc
var FunctionCallTestImplementation FunctionCallTestFunc

//export executeOrderedGo
func executeOrderedGo(command *C.BFT_BYTE, size C.int, output **C.BFT_BYTE) C.int {
	saida := ExecuteOrderedImplementation(C.GoBytes(unsafe.Pointer(command),size));
	*output = (*C.BFT_BYTE)(bftsmartallocate(uint(len(saida))));
        copy( (*[1<<30]byte)(unsafe.Pointer(*output))[0:len(saida)], saida);
	return C.int(len(saida));
}

//export executeUnorderedGo
func executeUnorderedGo(command *C.BFT_BYTE, size C.int, output **C.BFT_BYTE) C.int {
        saida := ExecuteUnorderedImplementation(C.GoBytes(unsafe.Pointer(command),size));
        *output = (*C.BFT_BYTE)(bftsmartallocate(uint(len(saida))));
        copy( (*[1<<30]byte)(unsafe.Pointer(*output))[0:len(saida)], saida);
        return C.int(len(saida));
}

//export getSnapshotGo
func getSnapshotGo(output **C.BFT_BYTE) C.int {
        saida := GetSnapImplementation()
        *output = (*C.BFT_BYTE)(bftsmartallocate(uint(len(saida))));
        copy( (*[1<<30]byte)(unsafe.Pointer(*output))[0:len(saida)], saida);
        return C.int(len(saida));
}

//export installSnapshotGo
func installSnapshotGo(data *C.BFT_BYTE, size C.int) {
        InstallSnapImplementation(C.GoBytes(unsafe.Pointer(data),size));
}

//export releaseBufferGo
func releaseBufferGo(impl *C.BFT_BYTE) {
	bftsmartrelease(unsafe.Pointer(impl))	
}

//export functionCallTestGo
func functionCallTestGo() {

}

func CarregarJvm() int {                          
	return int(C.carregarJvm())
}                                                 
                                                  
func CreateServiceProxy(id int) int {             
	return int(C.createServiceProxy(C.int(id)))
}                                                 
                                                  
func InvokeOrdered(command []byte) []byte {       
        output := (*C.BFT_BYTE)(bftsmartallocate(100));        
        c_command := (*C.BFT_BYTE)(bftsmartallocate(uint(len(command))))
        copy( (*[1<<30]byte)(unsafe.Pointer(c_command))[0:len(command)], command);
        
        tam_saida := C.invokeOrdered(c_command  , C.int( len(command) ), output);

        return C.GoBytes(unsafe.Pointer(output), tam_saida)
}                                                 
                                                  
func InvokeUnordered(command []byte) []byte {     
        output := (*C.BFT_BYTE)(bftsmartallocate(100));
        c_command := (*C.BFT_BYTE)(bftsmartallocate(uint(len(command))))
        copy( (*[1<<30]byte)(unsafe.Pointer(c_command))[0:len(command)], command);

        tam_saida := C.invokeUnordered(c_command  , C.int( len(command) ), output);

        return C.GoBytes(unsafe.Pointer(output), tam_saida)
}                                                 
                                                  
func FinalizarJvm() {                             
	C.finalizarJvm()
}                                                 
                                                  
func StartServiceReplica(id int) int {            
	return int(C.startServiceReplica(C.int(id)))
}                                                 

func bftsmartallocate(tamanho uint) unsafe.Pointer {
	return C.bftsmartallocate(C.size_t(tamanho))
}

func bftsmartrelease(ponteiro unsafe.Pointer) {
	C.bftsmartrelease(ponteiro)
}

func SetClasspath(cl string) {
	C.setClasspath(C.CString(cl))
}
