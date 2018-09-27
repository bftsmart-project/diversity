package bftsmartserver

import "bftsmart"
import "time"

type replica_definition interface {
    ExecuteOrdered([]byte) []byte
    ExecuteUnordered([]byte) []byte
    GetSnapshot() []byte
    InstallSnapshot([]byte)
}

func StartServiceReplica(id int, classpath string, replica replica_definition) int {  
	bftsmart.ExecuteOrderedImplementation = replica.ExecuteOrdered
	bftsmart.ExecuteUnorderedImplementation = replica.ExecuteUnordered
	bftsmart.GetSnapImplementation = replica.GetSnapshot
	bftsmart.InstallSnapImplementation = replica.InstallSnapshot
	bftsmart.Init()        
	bftsmart.SetClasspath(classpath)
	bftsmart.CarregarJvm() 
	ret := int(bftsmart.StartServiceReplica(id))
	for true {
		time.Sleep(1 * time.Second)
	}
	return ret
	
}                                                              
                                                  
func FinalizarJvm() {                             
	bftsmart.FinalizarJvm()
}                                                 
                                                  

