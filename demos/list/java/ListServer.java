/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package bftsmart.diversity.demos.list;

import bftbench.EstadoOuterClass;
import bftbench.EstadoOuterClass.Estado;
import bftbench.RequestOuterClass;
import bftbench.ResponseOuterClass;
import bftsmart.tom.MessageContext;
import bftsmart.tom.ServiceReplica;
import bftsmart.tom.server.defaultservices.DefaultSingleRecoverable;
import bftsmart.tom.util.Storage;
import com.google.protobuf.InvalidProtocolBufferException;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author alchieri
 */
public class ListServer extends DefaultSingleRecoverable {

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
    private Storage executeOrderedTime = null;
    private int count = 0;

    public ListServer(int id, int intervalo) {
        this.interval = intervalo;
         if (interval > 0) {
        totalLatency = new Storage(interval);
        consensusLatency = new Storage(interval);
        preConsLatency = new Storage(interval);
        posConsLatency = new Storage(interval);
        proposeLatency = new Storage(interval);
        writeLatency = new Storage(interval);
        acceptLatency = new Storage(interval);
        executeOrderedTime = new Storage(interval);

         }
        new ServiceReplica(id, this, this);
        //estado = Testcount.Counter.newBuilder().setValue(0).build();
    }

    private byte[] execute(byte[] command, MessageContext msgCtx) {
        try {

            computeStatistics(msgCtx);
            bftbench.RequestOuterClass.Request rqst
                    = bftbench.RequestOuterClass.Request.parseFrom(command);

            ResponseOuterClass.Response.Builder reqst;
            reqst = ResponseOuterClass.Response
                    .newBuilder();

                       // .setAction(RequestOuterClass.Request.RequestType.REMOVE)
            // .setValue(index)
            // .build();
            switch (rqst.getAction()) {
                case ADD:
                    //System.out.println("Add " + rqst.getValue());
                    int x = lista.buscarIndice(rqst.getValue());
                    reqst.setBoolResponse(false);
                    if (x < 0) {
                        reqst.setBoolResponse(true);
                        lista.insereFinal(rqst.getValue());
                    }
                    

                    break;
                case CONTAINS:
                    reqst.setBoolResponse(lista.buscarIndice(rqst.getValue()) >= 0);
                    //System.out.println("Contains " + rqst.getValue());
                    break;
                case REMOVE:
                    //System.out.println("Remove " + rqst.getValue());
                    int y = lista.buscarIndice(rqst.getValue());
                    reqst.setBoolResponse(true);
                    if (y < 0) reqst.setBoolResponse(false);
                    else lista.remover(y);
                    
                    break;
                case GET:
                    reqst.setIntResponse(lista.buscarValor(rqst.getValue()));
                   
                    break;
                case SIZE:
                    reqst.setIntResponse(lista.tamanhoLista());
                    //System.out.println("Size ");
                    break;
                default:
                    break;
            }

            return reqst.build().toByteArray();
        } catch (InvalidProtocolBufferException ex) {
            throw new RuntimeException(ex);
        }

    }

    @Override
    public byte[] appExecuteOrdered(byte[] command, MessageContext msgCtx) {
	count++;
	if (interval > 0 && count % interval == 0)
{
       System.out.println("e // Average time for " + interval + " executions (-10%) = " + executeOrderedTime.getAverage(true) / 1000 + " us ");
           System.out.println("e // Standard desviation for " + interval + " executions (-10%) = " + executeOrderedTime.getDP(true) / 1000 + " us ");
      System.out.println("e // Average time for " + interval + " executions (all samples) = " + executeOrderedTime.getAverage(false) / 1000 + " us ");
    System.out.println("e // Standard desviation for " + interval + " executions (all samples) = " + executeOrderedTime.getDP(false) / 1000 + " us ");
        System.out.println("e // Maximum time for " + interval + " executions (all samples) = " + executeOrderedTime.getMax(false) / 1000 + " us ");
            executeOrderedTime.reset();
  
}
long x = System.nanoTime();
byte[] p = execute(command, msgCtx);
if (interval > 0) {
executeOrderedTime.store(System.nanoTime() - x);
}
return p;
    }

    @Override
    public byte[] executeUnordered(byte[] command, MessageContext msgCtx) {
        return execute(command, msgCtx);
    }

    @Override
    public void installSnapshot(byte[] state) {
        try {
            Estado estado = EstadoOuterClass.Estado.parseFrom(state);
            lista = new ListaNormal();
            int x = estado.getListaCount();
            for (int i = 0; i < x; i++) {
                lista.insereFinal(estado.getLista(i));
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }

    }

    @Override
    public byte[] getSnapshot() {
        Estado.Builder estbuil = Estado.newBuilder();

       ElementoLista atual = lista.inicio;
        while (atual != null) {
            estbuil.addLista(atual.dado);
            atual = atual.proximo;
        }

        return estbuil.build().toByteArray();
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
