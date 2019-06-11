package main

import "bftsmartserver"
import "os"
import "fmt"
import "bftbench"
import "github.com/golang/protobuf/proto"
import "strconv"
import "time"

type replica struct {
    resp_size int
    idreplica int
    interval int
    // Instrumentation for throughput calculation
    rqst_count int
    start_time time.Time
    max_tp float64
}

func (r * replica) ExecuteOrdered(command []byte) []byte {
    return r.execute(command);
}

func (r * replica) ExecuteUnordered(command []byte) []byte {
    return r.execute(command);
}

func (r * replica) GetSnapshot() []byte {
    est := new(bftbench.Estado)
    data, err := proto.Marshal(est)
    checkError(err)
    return data;
}

func (r * replica) InstallSnapshot(state []byte) {
    est := new(bftbench.Estado)
    err := proto.Unmarshal(state, est)
    checkError(err)
}

func (r * replica) execute(command []byte) []byte {
	if r.start_time.IsZero() {
	   r.start_time = time.Now();
        }
	req := new(bftbench.Request)
	err := proto.Unmarshal(command, req)
	checkError(err)

	rsp := new(bftbench.Response)

	var resp []byte = make([]byte, r.resp_size);
	for i, _ := range resp {
		resp[i] = 0xFF;
        }
	rsp.Data = resp;

	data, err := proto.Marshal(rsp)
	checkError(err)
	// Instrumentation for throughput calculation
        r.rqst_count++;
        if r.rqst_count >= r.interval {
               since := time.Since(r.start_time);
	       tp := float64(r.rqst_count) / since.Seconds();
	       fmt.Println("Throughput: ", float64(r.rqst_count) / since.Seconds(), " / s")
               if tp > r.max_tp {
			r.max_tp = tp;

	       }
	       fmt.Println("Max Throughput: ", r.max_tp)
	       if r.idreplica == 0 {
	         fmt.Println("MAXTHRO: ", r.max_tp);
	       }
	       r.rqst_count = 0; 
	       r.start_time = time.Now();
	}
	return data;
}

func main() {
	i, err := strconv.Atoi(os.Args[1])
	checkError(err)
	s, err := strconv.Atoi(os.Args[3])
	checkError(err)
        inte, err := strconv.Atoi(os.Args[4])
        checkError(err)
	r := replica{ resp_size: s, interval: inte, max_tp: 0, idreplica: i}
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
