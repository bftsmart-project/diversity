/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package bftsmart.demo.microbenchmarks.parallel.list;

import bftsmart.tom.MessageContext;
import bftsmart.tom.ServiceReplica;
import bftsmart.tom.server.defaultservices.DefaultSingleRecoverable;
import bftsmart.tom.util.Storage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author alchieri
 */
public class TCCListServerPure extends DefaultSingleRecoverable {

    static ListaNormal lista = new ListaNormal();
    private int iterations = 0;
    private long throughputMeasurementStartTime = System.currentTimeMillis();
    private int interval;
    private float maxTp = -1;
    private boolean context = false;
    private int initialEntries = 0;
    private Storage totalLatency = null;
    private Storage consensusLatency = null;
    private Storage preConsLatency = null;
    private Storage posConsLatency = null;
    private Storage proposeLatency = null;
    private Storage writeLatency = null;
    private Storage acceptLatency = null;

    public TCCListServerPure(int id, int intervalo) {
        this.interval = intervalo;
        if (interval > 0) {
        totalLatency = new Storage(interval);
        consensusLatency = new Storage(interval);
        preConsLatency = new Storage(interval);
        posConsLatency = new Storage(interval);
        proposeLatency = new Storage(interval);
        writeLatency = new Storage(interval);
        acceptLatency = new Storage(interval);
        }
        new ServiceReplica(id, this, this);
        //estado = Testcount.Counter.newBuilder().setValue(0).build();
    }

    private byte[] execute(byte[] command, MessageContext msgCtx) {
        try {

            computeStatistics(msgCtx);
            
            ByteArrayInputStream is = new ByteArrayInputStream(command);
            DataInputStream dis = new DataInputStream(is);
            int comando = dis.readInt();
            int valor;
            
            ByteArrayOutputStream os = new ByteArrayOutputStream();
            DataOutputStream  dos = new DataOutputStream (os);
                       // .setAction(RequestOuterClass.Request.RequestType.REMOVE)
            // .setValue(index)
            // .build();
            switch (comando) {
                case TipoRequest.ADD:
                    valor = dis.readInt();
                    //System.out.println("Add " + valor);
                    if (lista.buscarIndice(valor) < 0) {
                        lista.insereFinal(valor);
                        dos.writeBoolean(true);
                    } else {
                        dos.writeBoolean(false);
                    }

                    break;
                case TipoRequest.CONTAINS:
                    valor = dis.readInt();
                    dos.writeBoolean(lista.buscarIndice(valor) > 0);
                    //System.out.println("Contains " + valor);
                    break;
                case TipoRequest.REMOVE:
                    valor = dis.readInt();
                    //System.out.println("Remove " + valor);
                    int y = lista.buscarIndice(valor);
                    if (y >= 0) {
                       lista.remover(y);
                        dos.writeBoolean(true);
                    } else {
                        dos.writeBoolean(false);
                    }
                    break;
                case TipoRequest.GET:
                    valor = dis.readInt();
                    dos.writeInt(lista.buscarValor(valor));
                    break;
                case TipoRequest.SIZE:
                    dos.writeInt(lista.tamanhoLista());
                    //System.out.println("Size ");
                    break;
                default:
                    break;
            }
            dos.close();
            return os.toByteArray();
        } catch (Exception ex) {
            throw new RuntimeException(ex);
        }

    }

    @Override
    public byte[] appExecuteOrdered(byte[] command, MessageContext msgCtx) {
        return execute(command, msgCtx);
    }

    @Override
    public byte[] executeUnordered(byte[] command, MessageContext msgCtx) {
        return execute(command, msgCtx);
    }

    @Override
    public void installSnapshot(byte[] state) {
        try {
            ByteArrayInputStream is = new ByteArrayInputStream(state);
            DataInputStream dis = new DataInputStream(is);
         
            int x = dis.readInt();

            lista = new ListaNormal();
            for (int i = 0; i < x; i++) {
                lista.insereFinal(dis.readInt());
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }

    }

    @Override
    public byte[] getSnapshot() {
        ByteArrayOutputStream os = new ByteArrayOutputStream();
        DataOutputStream  dos = new DataOutputStream (os);

        int x = lista.tamanhoLista();
        try {
            dos.writeInt(x);
        } catch (IOException ex) {
          throw new RuntimeException(ex);
        }
        
       ElementoLista atual = lista.inicio;
        while (atual != null) {
try {
                dos.writeInt(atual.dado); 
} catch (IOException ex) {
throw new RuntimeException(ex);
}
            atual = atual.proximo;
        }

        try {
            dos.close();
        } catch (IOException ex) {
             throw new RuntimeException(ex);
        }
        return  os.toByteArray();
    }

    public void computeStatistics(MessageContext msgCtx) {
        /*for(int i = 0; i < 10; i++){
         int x = (int)(Math.random()*10000000);
         t.get(String.valueOf("TESTE"+(x)));
         }*/
        if (this.interval == 0) return;
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

    }

}
