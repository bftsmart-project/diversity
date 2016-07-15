package main

import "bftsmartserver"
import "os"
import "fmt"
import "bftbench"
import "github.com/golang/protobuf/proto"
import "strconv"

type replica struct {
    state []int32
}

func (r replica) ExecuteOrdered(command []byte) []byte {
    return r.execute(command);
}

func (r replica) ExecuteUnordered(command []byte) []byte {
    return r.execute(command);
}

func (r replica) GetSnapshot() []byte {
    return []byte{}
}

func (r replica) InstallSnapshot(state []byte) {
    
}

func (r replica) execute(command []byte) []byte {
	req := new(bftbench.Request)
	err := proto.Unmarshal(command, req)
	checkError(err)
	var res bool;
	var achou bool;
	rsp := new(bftbench.Response) 

	if req.GetAction() == bftbench.Request_ADD {
        val := req.GetValue()
        res = false;
        achou = false;
        for _,element := range r.state {
            if element == val {
                achou = true
                break
            }
        }
        if achou == false {
            r.state = append(r.state, val)
            res = true;
        }
        rsp.BoolResponse = &res
        data, err := proto.Marshal(rsp)
        checkError(err)
        return data
	} else if req.GetAction() == bftbench.Request_GET {
	    val := req.GetValue()
        if val >= int32(len(r.state)) {
            val = -1
        } else {
            val = r.state[val]
        }
        rsp.IntResponse = &val
        data, err := proto.Marshal(rsp)
        checkError(err)
        return data
	} else if req.GetAction() == bftbench.Request_REMOVE {
	    val := req.GetValue()
        res = false;
        achou = false;
        for index,element := range r.state {
            if element == val {
                achou = true
                val = int32(index)
                break
            }
        }
        if achou == true {
            r.state = append(r.state[:val], r.state[val+1:]...)
            res = true;
        }
        rsp.BoolResponse = &res
        data, err := proto.Marshal(rsp)
        checkError(err)
        return data
	} else if req.GetAction() == bftbench.Request_SIZE {
	    val := int32(len(r.state))
        rsp.IntResponse = &val
        data, err := proto.Marshal(rsp)
        checkError(err)
        return data
	} else {    // contains
	    val := req.GetValue()
        res = false;
        achou = false;
        for _,element := range r.state {
            if element == val {
                achou = true
                break
            }
        }
        if achou == true {
            res = true;
        }
        rsp.BoolResponse = &res
        data, err := proto.Marshal(rsp)
        checkError(err)
        return data
	}
}

func main() {
	r := replica{ state: []int32{} }
	i, err := strconv.Atoi(os.Args[1])
	checkError(err)
	bftsmartserver.StartServiceReplica(i, os.Args[2], r);
	bftsmartserver.FinalizarJvm()
}


func checkError(err error){
	if err != nil {
		fmt.Fprintf(os.Stderr, "Fatal error: %s", err.Error())
		os.Exit(1)
	}
}
