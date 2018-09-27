package main

import "bftsmartserver"
import "os"
import "fmt"
import "strconv"
import "sort"

type replica struct {
    state map[string]string
    resp_size int
}

func (r * replica) ExecuteOrdered(command []byte) []byte {
    return r.execute(command);
}

func (r * replica) ExecuteUnordered(command []byte) []byte {
    return r.execute(command);
}

func (r * replica) GetSnapshot() []byte {
    return make([]byte, 0)
}

func (r * replica) InstallSnapshot(state []byte) {
}

func (r * replica) execute(command []byte) []byte {
	var resp []byte = make([]byte, r.resp_size);
	for i, _ : range resp {
		resp[i] = 0xFF;
        }
	return resp;
}

func main() {
	i, err := strconv.Atoi(os.Args[1])
	checkError(err)
	s, err := strconv.Atoi(os.Args[3])
	checkError(err)
	r := replica{ state: make(map[string]string), resp_size: s }
	bftsmartserver.StartServiceReplica(i, os.Args[2], &r);
	bftsmartserver.FinalizarJvm()
	fmt.Println("main finalizou")
}


func checkError(err error){
	if err != nil {
		fmt.Fprintf(os.Stderr, "Fatal error: %s", err.Error())
		os.Exit(1)
	}
}
