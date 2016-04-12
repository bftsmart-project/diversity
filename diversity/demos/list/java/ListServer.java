/**
 * Copyright (c) 2007-2013 Alysson Bessani, Eduardo Alchieri, Paulo Sousa, and
 * the authors indicated in the @author tags
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package bftsmart.demo.microbenchmarks.parallel.list;

import bftsmart.statemanagement.ApplicationState;
import bftsmart.statemanagement.StateManager;
import bftsmart.statemanagement.strategy.StandardStateManager;
import bftsmart.tom.MessageContext;
import bftsmart.tom.parallelism.ParallelMapping;
import bftsmart.tom.parallelism.ParallelServiceReplica;
import bftsmart.tom.ReplicaContext;
import bftsmart.tom.ServiceReplica;
import bftsmart.tom.server.Recoverable;
import bftsmart.tom.server.SingleExecutable;

import bftsmart.tom.util.Storage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.PrintWriter;

import java.util.LinkedList;
import java.util.List;

import java.util.logging.Level;

/**
 * Simple server that just acknowledge the reception of a request.
 */
public final class ListServer implements SingleExecutable, Recoverable {

    private int interval;
    private float maxTp = -1;
    private boolean context;

    private byte[] state;

    private int iterations = 0;
    private long throughputMeasurementStartTime = System.currentTimeMillis();

    private Storage totalLatency = null;
    private Storage consensusLatency = null;
    private Storage preConsLatency = null;
    private Storage posConsLatency = null;
    private Storage proposeLatency = null;
    private Storage writeLatency = null;
    private Storage acceptLatency = null;
    private ServiceReplica replica;
    private ReplicaContext replicaContext;

    private StateManager stateManager;

    //private Map<Integer, Object> t = new TreeMap<Integer, Object>();
    //private Map<Integer, Object> t = new BST();
    private List<Integer> l = new LinkedList<Integer>();
    private int initialEntries = 0;
    private int nT;
   // private PrivateKey privK;
    //private PublicKey pubK;
    // private byte[] signature;
    // private byte[] msg = new byte[256];

    public ListServer(int id, int interval, int numThreads, int entries, int stateSize, boolean context) {

        if (numThreads == 0) {
            System.out.println("Replica in sequential execution model.");
            replica = new ServiceReplica(id, this, this);
        } else {
            System.out.println("Replica in parallel execution model.");
            //ParallelMapping m = new ParallelMapping(numThreads);
            //m.addMultiGroup(20, new int[]{4,5});
            replica = new ParallelServiceReplica(id, numThreads, this, this);
        }
        this.nT = numThreads;
        this.interval = interval;
        this.context = context;

        this.state = new byte[stateSize];

        for (int i = 0; i < stateSize; i++) {
            state[i] = (byte) i;
        }

        totalLatency = new Storage(interval);
        consensusLatency = new Storage(interval);
        preConsLatency = new Storage(interval);
        posConsLatency = new Storage(interval);
        proposeLatency = new Storage(interval);
        writeLatency = new Storage(interval);
        acceptLatency = new Storage(interval);

        initialEntries = entries;
        for (int i = 0; i < entries; i++) {
            l.add(i);
            //System.out.println("adicionando key: "+i);
        }

        /*  try {
         KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance("RSA");
         keyPairGenerator.initialize(1024);
         java.security.KeyPair keyPair = keyPairGenerator.genKeyPair();

         pubK = keyPair.getPublic();
         privK = keyPair.getPrivate();

         } catch (Exception e) {
         e.printStackTrace();
         }
         sign();*/
        System.out.println("Servidor inicializado");
    }

    public void setReplicaContext(ReplicaContext replicaContext) {
        this.replicaContext = replicaContext;
    }

    public byte[] executeOrdered(byte[] command, MessageContext msgCtx) {
        return execute(command, msgCtx);
    }

    public byte[] executeUnordered(byte[] command, MessageContext msgCtx) {
        return execute(command, msgCtx);
    }

    public byte[] execute(byte[] command, MessageContext msgCtx) {

        computeStatistics(msgCtx);

        try {
            ByteArrayInputStream in = new ByteArrayInputStream(command);
            ByteArrayOutputStream out = null;
            byte[] reply = null;
            int cmd = new DataInputStream(in).readInt();
            switch (cmd) {
                //operations on the list
                case BFTList.ADD:
                    Integer value = (Integer) new ObjectInputStream(in).readObject();
                    //System.out.println("add received: " +value.toString());
                    boolean ret = false;
                    if(!l.contains(value)){
                        ret = l.add(value);
                    }
                    out = new ByteArrayOutputStream();
                    ObjectOutputStream out1 = new ObjectOutputStream(out);
                    out1.writeBoolean(ret);
                    out.flush();
                    out1.flush();
                    reply = out.toByteArray();
                    break;
                case BFTList.REMOVE:
                    value = (Integer) new ObjectInputStream(in).readObject();
                    ret = l.remove(value);
                    out = new ByteArrayOutputStream();
                    out1 = new ObjectOutputStream(out);
                    out1.writeBoolean(ret);
                    out.flush();
                    out1.flush();

                    reply = out.toByteArray();
                    break;
                case BFTList.SIZE:
                    out = new ByteArrayOutputStream();
                    new DataOutputStream(out).writeInt(l.size());
                    reply = out.toByteArray();
                    break;
                case BFTList.CONTAINS:
                    value = (Integer) new ObjectInputStream(in).readObject();
                    out = new ByteArrayOutputStream();
                    out1 = new ObjectOutputStream(out);
                    out1.writeBoolean(l.contains(value));

                    out.flush();
                    out1.flush();

                    reply = out.toByteArray();
                    break;
                case BFTList.GET:

                    int index = new DataInputStream(in).readInt();
                    //long last_send_instant = System.nanoTime();
                    Integer r = null;
                    if (index > l.size()) {
                        r = new Integer(-1);
                    } else {
                        r = l.get(index);
                    }

                    //long time = System.nanoTime() - last_send_instant;
                    //System.out.println("tempo de busca: " + time);
                    out = new ByteArrayOutputStream();
                    out1 = new ObjectOutputStream(out);
                    out1.writeObject(r);

                    reply = out.toByteArray();

                    break;
            }
            return reply;
        } catch (Exception ex) {
            java.util.logging.Logger.getLogger(ListServer.class.getName()).log(Level.SEVERE, null, ex);
            return null;
        }
    }

    public void computeStatistics(MessageContext msgCtx) {
        /*for(int i = 0; i < 10; i++){
         int x = (int)(Math.random()*10000000);
         t.get(String.valueOf("TESTE"+(x)));
         }*/

        boolean readOnly = false;

        iterations++;

        if (msgCtx != null && msgCtx.getFirstInBatch() != null) {

            readOnly = msgCtx.readOnly;

            msgCtx.getFirstInBatch().executedTime = System.nanoTime();

            totalLatency.store(msgCtx.getFirstInBatch().executedTime - msgCtx.getFirstInBatch().receptionTime);

            if (readOnly == false) {

                consensusLatency.store(msgCtx.getFirstInBatch().decisionTime - msgCtx.getFirstInBatch().consensusStartTime);
                long temp = msgCtx.getFirstInBatch().consensusStartTime - msgCtx.getFirstInBatch().receptionTime;
                preConsLatency.store(temp > 0 ? temp : 0);
                posConsLatency.store(msgCtx.getFirstInBatch().executedTime - msgCtx.getFirstInBatch().decisionTime);
                proposeLatency.store(msgCtx.getFirstInBatch().writeSentTime - msgCtx.getFirstInBatch().consensusStartTime);
                writeLatency.store(msgCtx.getFirstInBatch().acceptSentTime - msgCtx.getFirstInBatch().writeSentTime);
                acceptLatency.store(msgCtx.getFirstInBatch().decisionTime - msgCtx.getFirstInBatch().acceptSentTime);

            } else {

                consensusLatency.store(0);
                preConsLatency.store(0);
                posConsLatency.store(0);
                proposeLatency.store(0);
                writeLatency.store(0);
                acceptLatency.store(0);

            }

        } else {

            consensusLatency.store(0);
            preConsLatency.store(0);
            posConsLatency.store(0);
            proposeLatency.store(0);
            writeLatency.store(0);
            acceptLatency.store(0);

        }

        float tp = -1;
        if (iterations % interval == 0) {
            if (context) {
                System.out.println("--- (Context)  iterations: " + iterations + " // regency: " + msgCtx.getRegency() + " // consensus: " + msgCtx.getConsensusId() + " ---");
            }

            System.out.println("--- Measurements after " + iterations + " ops (" + interval + " samples) ---");

            tp = (float) (interval * 1000 / (float) (System.currentTimeMillis() - throughputMeasurementStartTime));

            if (tp > maxTp) {
                maxTp = tp;
            }

            System.out.println("Throughput = " + tp + " operations/sec (Maximum observed: " + maxTp + " ops/sec)");

            System.out.println("Total latency = " + totalLatency.getAverage(false) / 1000 + " (+/- " + (long) totalLatency.getDP(false) / 1000 + ") us ");
            totalLatency.reset();
            System.out.println("Consensus latency = " + consensusLatency.getAverage(false) / 1000 + " (+/- " + (long) consensusLatency.getDP(false) / 1000 + ") us ");
            consensusLatency.reset();
            System.out.println("Pre-consensus latency = " + preConsLatency.getAverage(false) / 1000 + " (+/- " + (long) preConsLatency.getDP(false) / 1000 + ") us ");
            preConsLatency.reset();
            System.out.println("Pos-consensus latency = " + posConsLatency.getAverage(false) / 1000 + " (+/- " + (long) posConsLatency.getDP(false) / 1000 + ") us ");
            posConsLatency.reset();
            System.out.println("Propose latency = " + proposeLatency.getAverage(false) / 1000 + " (+/- " + (long) proposeLatency.getDP(false) / 1000 + ") us ");
            proposeLatency.reset();
            System.out.println("Write latency = " + writeLatency.getAverage(false) / 1000 + " (+/- " + (long) writeLatency.getDP(false) / 1000 + ") us ");
            writeLatency.reset();
            System.out.println("Accept latency = " + acceptLatency.getAverage(false) / 1000 + " (+/- " + (long) acceptLatency.getDP(false) / 1000 + ") us ");
            acceptLatency.reset();

            throughputMeasurementStartTime = System.currentTimeMillis();
        }

        if (iterations == 110000) {
            //resetar
            System.out.println("Reiniciando o servidor para novo experimento");
            try {
                File f = new File("resultado_"+nT+".txt");
                FileWriter fw = new FileWriter(f);
                PrintWriter pw = new PrintWriter(fw);
                
                pw.println("Throughput = " + tp + " operations/sec (Maximum observed: " + maxTp + " ops/sec)");
                
                pw.flush();
                fw.flush();
                
                fw.close();
                pw.close();
            } catch (Exception e) {
                e.printStackTrace();
                System.exit(0);
            }

            maxTp = 0;
            iterations = 0;
            l.clear();
            for (int i = 0; i < initialEntries; i++) {
                l.add(i);
                //System.out.println("adicionando key: "+i);
            }

            System.out.println("Servidor reiniciado");
        }

    }

    public static void main(String[] args) {
        if (args.length < 6) {
            System.out.println("Usage: ... ParallelThroughputLatencyServer <processId> <measurement interval> <num threads> <initial entries> <state size> <context?>");
            System.exit(-1);
        }

        int processId = Integer.parseInt(args[0]);
        int interval = Integer.parseInt(args[1]);
        int nt = Integer.parseInt(args[2]);
        int entries = Integer.parseInt(args[3]);
        int stateSize = Integer.parseInt(args[4]);
        boolean context = Boolean.parseBoolean(args[5]);

        new ListServer(processId, interval, nt, entries, stateSize, context);
    }

    public byte[] getState() {
        return state;
    }

    public void setState(byte[] state) {
    }

    @Override
    public ApplicationState getState(int eid, boolean sendState) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public int setState(ApplicationState state) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public StateManager getStateManager() {
        if (stateManager == null) {
            stateManager = new StandardStateManager();
        }
        return stateManager;
    }

}
