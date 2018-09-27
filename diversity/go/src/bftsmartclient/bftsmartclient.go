package bftsmartclient

import "bftsmart"
                                            
func CreateServiceProxy(id int, clspath string) int {             
	bftsmart.SetClasspath(clspath)
	bftsmart.CarregarJvm()
	return bftsmart.CreateServiceProxy(id)
}                                                 
                                                  
func InvokeOrdered(command []byte) []byte {       
        return bftsmart.InvokeOrdered(command)
}
                                                  
func InvokeUnordered(command []byte) []byte {     
        return bftsmart.InvokeUnordered(command)
}                                                 
                                                  
func FinalizarJvm() {                             
	bftsmart.FinalizarJvm()
}                                                 
