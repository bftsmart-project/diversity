package bftsmartserver

import "bftsmart"

type replica_definition interface {
    executeOrdered([]byte) []byte
    executeUnordered([]byte) []byte
    getSnapshot() []byte
    installSnapshot([]byte)
}

func StartServiceReplica(id int, classpath string, replica replica_definition) int {  
	bftsmart.ExecuteOrderedImplementation = replica.executeOrdered
	bftsmart.ExecuteUnorderedImplementation = replica.executeUnordered
	bftsmart.GetSnapImplementation = replica.getSnapshot
	bftsmart.InstallSnapImplementation = replica.installSnapshot
	bftsmart.Init()        
	bftsmart.SetClasspath(classpath)
	bftsmart.CarregarJvm() 
	return int(bftsmart.StartServiceReplica(id))
}                                                              
                                                  
func FinalizarJvm() {                             
	bftsmart.FinalizarJvm()
}                                                 
                                                  

