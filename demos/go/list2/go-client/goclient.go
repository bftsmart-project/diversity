package main

import "bftsmartclient"
import "os"
import "fmt"
import "bftbench"
import "github.com/golang/protobuf/proto"
import "strconv"


func main() {
	i, err := strconv.Atoi(os.Args[1])
	checkError(err)
	bftsmartclient.CreateServiceProxy(i,os.Args[2])
	var text string
	fmt.Println("acao(add,remove,size,contains,get,sair) :")
	_,err = fmt.Scanln(&text)
	checkError(err)
	fmt.Println(text)
	fmt.Println("iniciando switch")
	for text != "sair" {
		fmt.Println("dentro switch")
		switch text {
			case "add":
				req := new(bftbench.Request)
				act := bftbench.Request_ADD
				req.Action = &act
				fmt.Println("value : ")
				fmt.Scanln(&text)
				i, err := strconv.Atoi(text)
				i32 := int32(i)
				checkError(err)
				req.Value = &i32
			        data, err := proto.Marshal(req) 
				checkError(err) 
				respd := bftsmartclient.InvokeOrdered(data)
				rsp := new(bftbench.Response)
				err = proto.Unmarshal(respd, rsp)
				fmt.Println(rsp.GetBoolResponse())
			case "remove": 
				req := new(bftbench.Request)
                                act := bftbench.Request_REMOVE
                                req.Action = &act
                                fmt.Println("value : ")
                                fmt.Scanln(&text)
                                i, err := strconv.Atoi(text)
                                i32 := int32(i)
                                checkError(err)
                                req.Value = &i32
                                data, err := proto.Marshal(req)
                                checkError(err)
                                respd := bftsmartclient.InvokeOrdered(data)
                                rsp := new(bftbench.Response)
                                err = proto.Unmarshal(respd, rsp)
                                fmt.Println(rsp.GetBoolResponse())
			case "size": 
				fmt.Println("executando size.")
				req := new(bftbench.Request)
                                act := bftbench.Request_SIZE
                                req.Action = &act
                                data, err := proto.Marshal(req)
                                checkError(err)
				fmt.Println("invoke ordered size")
                                respd := bftsmartclient.InvokeOrdered(data)
				fmt.Println("invoke ordered finalizado")
                                rsp := new(bftbench.Response)
                                err = proto.Unmarshal(respd, rsp)
                                fmt.Println(rsp.GetIntResponse())
			case "contains": 
				req := new(bftbench.Request)
                                act := bftbench.Request_CONTAINS
                                req.Action = &act
                                fmt.Println("value : ")
                                fmt.Scanln(&text)
                                i, err := strconv.Atoi(text)
                                i32 := int32(i)
                                checkError(err)
                                req.Value = &i32
                                data, err := proto.Marshal(req)
                                checkError(err)
                                respd := bftsmartclient.InvokeOrdered(data)
                                rsp := new(bftbench.Response)
                                err = proto.Unmarshal(respd, rsp)
                                fmt.Println(rsp.GetBoolResponse())
			case "get": 
				req := new(bftbench.Request)
                                act := bftbench.Request_GET
                                req.Action = &act
                                fmt.Println("value : ")
                                fmt.Scanln(&text)
                                i, err := strconv.Atoi(text)
                                i32 := int32(i)
                                checkError(err)
                                req.Value = &i32
                                data, err := proto.Marshal(req)
                                checkError(err)
                                respd := bftsmartclient.InvokeOrdered(data)
                                rsp := new(bftbench.Response)
                                err = proto.Unmarshal(respd, rsp)
                                fmt.Println(rsp.GetIntResponse())
		}
		fmt.Println("acao(add,remove,size,contains,get,sair) :")
		_,err = fmt.Scanln(&text)
		checkError(err)
	}
	fmt.Println("fim.")
	bftsmartclient.FinalizarJvm()
}


func checkError(err error){
	if err != nil {
		fmt.Fprintf(os.Stderr, "Fatal error: %s", err.Error())
		os.Exit(1)
	}
}
