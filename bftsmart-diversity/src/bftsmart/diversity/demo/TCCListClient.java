/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package bftsmart.demo.microbenchmarks.parallel.list;

import bftsmart.tom.parallelism.ParallelMapping;
import java.io.IOException;

import java.util.logging.Level;
import java.util.logging.Logger;

import bftsmart.tom.ServiceProxy;
import bftsmart.tom.core.messages.TOMMessageType;
import bftsmart.tom.util.Storage;
import java.util.Random;

import bftbench.RequestOuterClass;

/**
 *
 * @author caio
 */
public class TCCListClient {
    
    private static int VALUE_SIZE = 1024;
    public static int initId = 0;

    @SuppressWarnings("static-access")
    public static void main(String[] args) throws IOException {
        if (args.length < 8) {
            System.out.println("Usage: ... ListClient <num. threads> <process id> <number of operations> <interval> <maxIndex> <verbose?> <parallel?> <conflict percent> <opSel>");
            System.exit(-1);
        }

        int numThreads = Integer.parseInt(args[0]);
        initId = Integer.parseInt(args[1]);

        int numberOfOps = Integer.parseInt(args[2]);
        //int requestSize = Integer.parseInt(args[3]);
        int interval = Integer.parseInt(args[3]);
        int max = Integer.parseInt(args[4]);
        boolean verbose = Boolean.parseBoolean(args[5]);
        boolean parallel = Boolean.parseBoolean(args[6]);
        int percent = Integer.parseInt(args[7]);
        int opSel = Integer.parseInt(args[8]);
        Client[] c = new Client[numThreads];

        for (int i = 0; i < numThreads; i++) {
            try {
                Thread.sleep(100);
            } catch (InterruptedException ex) {
                Logger.getLogger(ListClient.class.getName()).log(Level.SEVERE, null, ex);
            }

            System.out.println("Launching client " + (initId + i));
            c[i] = new TCCListClient.Client(initId + i, numberOfOps, interval, max, verbose, parallel, percent, opSel);
            //c[i].start();
        }

        for (int i = 0; i < numThreads; i++) {

            c[i].start();
        }

        for (int i = 0; i < numThreads; i++) {

            try {
                c[i].join();
            } catch (InterruptedException ex) {
                ex.printStackTrace(System.err);
            }
        }

        System.exit(0);
    }

    static class Client extends Thread {

        int id;
        int numberOfOps;
        private ServiceProxy proxy = null;
        int interval;
        int opSelecionada;
        boolean verbose;
        //boolean dos;
        //ServiceProxy proxy;
        //byte[] request;

        int maxIndex;
        int percent;

        public Client(int id, int numberOfOps, int interval, int maxIndex, boolean verbose, boolean parallel, int percent, int opSelecionada) {
            super("Client " + id);
            this.opSelecionada = opSelecionada;
            this.id = id;
            this.numberOfOps = numberOfOps;
            this.percent = percent;

            this.interval = interval;

            this.verbose = verbose;
            //this.proxy = new ServiceProxy(id);
            //this.request = new byte[this.requestSize];
            this.maxIndex = maxIndex;

            proxy = new ServiceProxy(id);
            //this.dos = dos;
        }

        public void run() {

            System.out.println("Warm up...");

            int req = 0;
            Random rand = new Random(System.nanoTime() - this.id);
            WorkloadGenerator work = new WorkloadGenerator(this.percent, numberOfOps / 2);

            for (int i = 0; i <  
                    (Math.abs(this.opSelecionada*10 - 35)==15?(numberOfOps/2):(numberOfOps / 20)); i++, req++) {
                if (verbose) {
                    System.out.print("Sending req " + req + "...");
                }
                 // quando essa contda da 15 eopSlecionadonada eh 2 ou 5.
                int op = Math.abs(this.opSelecionada*10 - 35)!=15? 
                        work.getOperations()[i] : TipoRequest.ADD;
                              
                
                if (op == TipoRequest.ADD) {
                    int index = rand.nextInt(maxIndex * 2);

                    
                    RequestOuterClass.Request reqst = RequestOuterClass.Request
                        .newBuilder()
                        .setAction(RequestOuterClass.Request.RequestType.ADD)
                        .setValue(index)
                        .build();
                    
                    proxy.invokeOrdered(reqst.toByteArray());

                } else if (op == TipoRequest.CONTAINS) {

                    int index = rand.nextInt(maxIndex);

                    RequestOuterClass.Request reqst = RequestOuterClass.Request
                        .newBuilder()
                        .setAction(RequestOuterClass.Request.RequestType.CONTAINS)
                        .setValue(index)
                        .build();
                    
                    proxy.invokeOrdered(reqst.toByteArray());

                } else if (op == TipoRequest.GET) {

                    int index = rand.nextInt(maxIndex);

                   RequestOuterClass.Request reqst = RequestOuterClass.Request
                        .newBuilder()
                        .setAction(RequestOuterClass.Request.RequestType.GET)
                        .setValue(index)
                        .build();
                    
                    proxy.invokeOrdered(reqst.toByteArray());

                    //System.out.println("GET: "+ret);
                } else if (op == TipoRequest.REMOVE) {

                    int index = rand.nextInt(maxIndex);

                   RequestOuterClass.Request reqst = RequestOuterClass.Request
                        .newBuilder()
                        .setAction(RequestOuterClass.Request.RequestType.REMOVE)
                        .setValue(index)
                        .build();
                    
                    proxy.invokeOrdered(reqst.toByteArray());

                } else {//SIZE

                   RequestOuterClass.Request reqst = RequestOuterClass.Request
                        .newBuilder()
                        .setAction(RequestOuterClass.Request.RequestType.SIZE)
                        
                        .build();
                    
                    proxy.invokeOrdered(reqst.toByteArray());
                    //System.out.println("Size: "+s);

                }

                if (verbose) {
                    System.out.println(" sent!");
                }

                if (verbose && (req % 1000 == 0)) {
                    System.out.println(this.id + " // " + req + " operations sent!");
                }

            }

            Storage st = new Storage(numberOfOps / 2);

           // for (int j = 0; j < 5; j++) {
            System.out.println("Executing experiment for " + numberOfOps / 2 + " ops");

                //work = new WorkloadGenerator(j * 25, numberOfOps / 2);
            for (int i = 0; i < numberOfOps / 2; i++, req++) {

                if (verbose) {
                    System.out.print(this.id + " // Sending req " + req + "...");
                }

               // int op = work.getOperations()[i];
                  int op = this.opSelecionada;
                if (op == TipoRequest.ADD) {
                    int index = rand.nextInt(maxIndex * 2);
                    long last_send_instant = System.nanoTime();
                    RequestOuterClass.Request reqst = RequestOuterClass.Request
                        .newBuilder()
                        .setAction(RequestOuterClass.Request.RequestType.ADD)
                        .setValue(index)
                        .build();
                    
                    proxy.invokeOrdered(reqst.toByteArray());
                    st.store(System.nanoTime() - last_send_instant);
                } else if (op == TipoRequest.CONTAINS) {

                    int index = rand.nextInt(maxIndex);
                    long last_send_instant = System.nanoTime();
                    RequestOuterClass.Request reqst = RequestOuterClass.Request
                        .newBuilder()
                        .setAction(RequestOuterClass.Request.RequestType.CONTAINS)
                        .setValue(index)
                        .build();
                    
                    proxy.invokeOrdered(reqst.toByteArray());
                    st.store(System.nanoTime() - last_send_instant);

                } else if (op == TipoRequest.GET) {

                    int index = rand.nextInt(maxIndex);
                    long last_send_instant = System.nanoTime();
                    RequestOuterClass.Request reqst = RequestOuterClass.Request
                        .newBuilder()
                        .setAction(RequestOuterClass.Request.RequestType.GET)
                        .setValue(index)
                        .build();
                    
                    proxy.invokeOrdered(reqst.toByteArray());
                    st.store(System.nanoTime() - last_send_instant);

                } else if (op == TipoRequest.REMOVE) {

                    int index = rand.nextInt(maxIndex);
                    long last_send_instant = System.nanoTime();
                    RequestOuterClass.Request reqst = RequestOuterClass.Request
                        .newBuilder()
                        .setAction(RequestOuterClass.Request.RequestType.REMOVE)
                        .setValue(index)
                        .build();
                    
                    proxy.invokeOrdered(reqst.toByteArray());
                    st.store(System.nanoTime() - last_send_instant);

                } else {//SIZE
                    long last_send_instant = System.nanoTime();
                    RequestOuterClass.Request reqst = RequestOuterClass.Request
                        .newBuilder()
                        .setAction(RequestOuterClass.Request.RequestType.SIZE)
                       
                        .build();
                    
                    proxy.invokeOrdered(reqst.toByteArray());
                    st.store(System.nanoTime() - last_send_instant);
                }

                if (verbose) {
                    System.out.println(this.id + " // sent!");
                }

                // System.out.println("resultado lido= "+ ret.toString());
                if (interval > 0) {
                    try {
                        //sleeps interval ms before sending next request
                        Thread.sleep(interval);
                    } catch (InterruptedException ex) {
                    }
                }

                if (verbose && (req % 1000 == 0)) {
                    System.out.println(this.id + " // " + req + " operations sent!");
                }

            }

            if (id == initId) {
                System.out.println(this.id + " // Average time for " + numberOfOps / 2 + " executions (-10%) = " + st.getAverage(true) / 1000 + " us ");
                System.out.println(this.id + " // Standard desviation for " + numberOfOps / 2 + " executions (-10%) = " + st.getDP(true) / 1000 + " us ");
                System.out.println(this.id + " // Average time for " + numberOfOps / 2 + " executions (all samples) = " + st.getAverage(false) / 1000 + " us ");
                System.out.println(this.id + " // Standard desviation for " + numberOfOps / 2 + " executions (all samples) = " + st.getDP(false) / 1000 + " us ");
                System.out.println(this.id + " // Maximum time for " + numberOfOps / 2 + " executions (all samples) = " + st.getMax(false) / 1000 + " us ");
            }
	    try {
            Thread.sleep(3000); // sleep ao final, para dar tempo de
                                // finalizar toda comunicacao tcp com servidor
                                // evitando uma excecao ao final da execucao
            } catch (InterruptedException ex) {
		throw new RuntimeException(ex);
		}
                //proxy.close();
            //
               /* System.out.println("Terminou a execução para " + j * 25);
             try {
             //sleeps interval ms before sending next request
             Thread.sleep(120000);
             } catch (InterruptedException ex) {
             }*/
            //}
        }

    }
}
