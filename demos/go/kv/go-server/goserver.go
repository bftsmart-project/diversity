package main

import "bftsmartserver"
import "os"
import "fmt"
import "bftbench"
import "github.com/golang/protobuf/proto"
import "strconv"
import "sort"

type replica struct {
    state map[string]string
}

func (r * replica) ExecuteOrdered(command []byte) []byte {
    return r.execute(command);
}

func (r * replica) ExecuteUnordered(command []byte) []byte {
    return r.execute(command);
}

func (r * replica) GetSnapshot() []byte {
	est := new(bftbench.Estado)
	var keys []string
	for k := range r.state {
		keys = append(keys, k)
	}
	sort.Strings(keys)
	var mfe bftbench.MapFieldEntry
	for _, k := range keys {
		mfe = new(bftbench.MapFieldEntry)
		mfe.Key = k
		valr := r.state[k]
		mfe.Value = &valr
		est.Kv = append(est.Kv, mfe)
	}

    data, err := proto.Marshal(est)
    checkError(err)
    return data
}

func (r * replica) InstallSnapshot(state []byte) {
	est := new(bftbench.Estado)
	err := proto.Unmarshal(state, est)
	checkError(err)
	
	r.state = make(map[string]string)
	for _, mfe := range est.Kv {
		r.state[*(mfe.Key)] = *(mfe.Value)
	}
}

func (r * replica) execute(command []byte) []byte {
	req := new(bftbench.Request)
	err := proto.Unmarshal(command, req)
	checkError(err)
	var res bool;
	rsp := new(bftbench.Response) 

	if req.GetAction() == bftbench.Request_PUT {
        key := req.GetKey()
		val := req.GetValue()
		fmt.Println(val)
        res = true
		
		r.state[key] = val
		
        rsp.BoolResponse = &res
        data, err := proto.Marshal(rsp)
        checkError(err)
        return data
	} else if req.GetAction() == bftbench.Request_GET {
	    key := req.GetKey()
        if r.state[key] != "" {
            res = true
	    valr := r.state[key]
	    rsp.StringResponse = &valr
        } else {
            res = false;
        }
        rsp.BoolResponse = &res;		
        data, err := proto.Marshal(rsp)
        checkError(err)
        return data
	} else if req.GetAction() == bftbench.Request_DELETE {
	    key := req.GetKey()
        if r.state[key] != "" {
            res = true
			delete(r.state, key)
        } else {
            res = false
        }
        rsp.BoolResponse = &res
        data, err := proto.Marshal(rsp)
        checkError(err)
        return data
	} else { // LIST
		var keys []string
		for k := range r.state {
			keys = append(keys, k)
		}
		sort.Strings(keys)

		// To perform the opertion you want
		for _, k := range keys {
			rsp.ListResponse = append(rsp.ListResponse, k)
		}
		res = true
        rsp.BoolResponse = &res
        data, err := proto.Marshal(rsp)
        checkError(err)
        return data
	} 
}

func main() {
	r := replica{ state: make(map[string]string) }
	i, err := strconv.Atoi(os.Args[1])
	checkError(err)
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
