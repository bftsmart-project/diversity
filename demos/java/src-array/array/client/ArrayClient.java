package bftsmart.diversity.demos.array;

import bftsmart.tom.ServiceProxy;
import bftbench.EstadoOuterClass;
import bftbench.EstadoOuterClass.Estado;
import bftbench.RequestOuterClass;
import bftbench.ResponseOuterClass;
import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.ByteString;
import java.io.*;
import java.util.Collection;
import java.util.LinkedList;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import com.google.protobuf.ByteString;
import java.util.concurrent.Future;
import java.util.concurrent.LinkedBlockingQueue;

public class ArrayClient {
  public static LinkedBlockingQueue<Long> latencies;
  static class ClientThread extends Thread {
    int id;
    int numberOfOps;
    int requestSize;
    int interval;
    ServiceProxy proxy;
    byte[] request;

    public ClientThread(int id, int numberOfOps, int requestSize,
                        int interval) {
      super("Client " + id);

      this.id = id;
      this.numberOfOps = numberOfOps;
      this.requestSize = requestSize;
      this.interval = interval;
      this.proxy = new ServiceProxy(id);
      this.request = new byte[this.requestSize];

      for (int i = 0; i < requestSize; i++) {
        this.request[i] = (byte)0xFF;
      }
    }

    @Override
    public void run() {
      byte[] reply;
      int reqId;

      System.out.println("Executing experiment for " + numberOfOps / 2 +
                         " ops");

      for (int i = 0; i < numberOfOps; i++) {
        RequestOuterClass.Request.Builder reqst;
        reqst = RequestOuterClass.Request.newBuilder();
        reqst.setData(ByteString.copyFrom(request));
        long last_send_instant = System.nanoTime();
        reply = proxy.invokeOrdered(reqst.build().toByteArray());
        long latency = (System.nanoTime() - last_send_instant) / 1000;
        try {
          latencies.put(latency);
        } catch (InterruptedException ex) {
          throw new RuntimeException(ex);
        }
        try {
          bftbench.ResponseOuterClass.Response.parseFrom(reply);
        } catch (InvalidProtocolBufferException ex) {
          throw new RuntimeException(ex);
        }
      }
      proxy.close();
    }
  }

  public static void main(String[] args) throws IOException {

    if (args.length < 5) {
      System.out.println(
          "Usage: ... ListClient <num. threads> <process id> <number of operations> <request size> <interval> ");
      System.exit(-1);
    }
    ArrayClient.latencies = new LinkedBlockingQueue<Long>();
    int numThreads = Integer.parseInt(args[0]);
    int initId = Integer.parseInt(args[1]);

    int numberOfOps = Integer.parseInt(args[2]);
    int requestSize = Integer.parseInt(args[3]);
    int interval = Integer.parseInt(args[4]);

    ClientThread[] clients = new ClientThread[numThreads];

    for (int i = 0; i < numThreads; i++) {
      try {
        Thread.sleep(100);
      } catch (InterruptedException ex) {
        // Logger.getLogger(ThroughputLatencyClient.class.getName()).log(Level.SEVERE,
        // null, ex);
      }

      System.out.println("Launching client " + (initId + i));
      clients[i] =
          new ClientThread(initId + i, numberOfOps, requestSize, interval);
    }

    ExecutorService exec = Executors.newFixedThreadPool(clients.length);
    Collection<Future<?>> tasks = new LinkedList();

    for (ClientThread c : clients) {
      tasks.add(exec.submit(c));
    }

    // wait for tasks completion
    for (Future<?> currTask : tasks) {
      try {
        currTask.get();
      } catch (InterruptedException ex) {
        System.out.println("InterruptedException" + ex.toString());
      } catch (ExecutionException ex) {
        System.out.println("ExecutionException" + ex.toString());
        // Logger.getLogger(ThroughputLatencyClient.class.getName()).log(Level.SEVERE,
        // null, ex);
      }
    }

    exec.shutdown();

    Long[] lats = latencies.toArray(new Long[0]);
    long sum = 0;
    for (int i = 0; i < lats.length; i++) {
      sum = sum + lats[i];
    }
    double average = sum / lats.length;
    System.out.println("LATENCY: " + (average) + "us");
    double sd = 0;
    for (int i = 0; i < lats.length; i++) {

      {
        sd += ((lats[i] - average) * (lats[i] - average));
      }
    }
    double standardDeviation = Math.sqrt(sd/(lats.length-1));
    System.out.println("STNDDEV: " + standardDeviation);
  }
}
