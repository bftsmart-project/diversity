/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package bftsmart.diversity;

import bftsmart.tom.MessageContext;
import bftsmart.tom.ServiceReplica;
import bftsmart.tom.server.defaultservices.DefaultSingleRecoverable;
import bftsmart.tom.util.Storage;
/**
 *
 * @author alchieri
 */
public class ServerWrapper extends DefaultSingleRecoverable {
    private Storage executeOrderedTime = null;
    
    private native byte[] executeOrderedNative(byte[] commando);

    private native byte[] executeUnorderedNative(byte[] commando);

    private native byte[] getSnapshotNative();

    private native void installSnapshotNative(byte[] commando);
    private final int interval = 50000;
    private int count;
    public ServerWrapper(int id) {
try {
        new ServiceReplica(id, this, this);
        executeOrderedTime = new Storage(interval);
        count = 0;
} catch (Exception ex) {
System.out.println(ex.getMessage());
throw ex;
}
    }

    @Override
    public byte[] appExecuteOrdered(byte[] command, MessageContext msgCtx) {
        //throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
        //System.out.println("executeOrdered");
        count++;
        if (count % interval == 0)
        {
       System.out.println(" // Average time for " + interval + " executions (-10%) = " + executeOrderedTime.getAverage(true) / 1000 + " us ");
           System.out.println(" // Standard desviation for " + interval + " executions (-10%) = " + executeOrderedTime.getDP(true) / 1000 + " us ");
      System.out.println(" // Average time for " + interval + " executions (all samples) = " + executeOrderedTime.getAverage(false) / 1000 + " us ");
    System.out.println(" // Standard desviation for " + interval + " executions (all samples) = " + executeOrderedTime.getDP(false) / 1000 + " us ");
        System.out.println(" // Maximum time for " + interval + " executions (all samples) = " + executeOrderedTime.getMax(false) / 1000 + " us ");
            executeOrderedTime.reset();
        }
        
        long x = System.nanoTime();
        byte[] p = executeOrderedNative(command);
        executeOrderedTime.store(System.nanoTime() - x);
        
        return p;
    }

    @Override
    public byte[] appExecuteUnordered(byte[] command, MessageContext msgCtx) {
        //throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
        //System.out.println("executeUnordered");
        return executeUnorderedNative(command);
    }

    @Override
    public void installSnapshot(byte[] state) {
        //throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
        //System.out.println("installSnap");
        installSnapshotNative(state);
    }

    @Override
    public byte[] getSnapshot() {
        //throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
        //System.out.println("getSnap");
        return getSnapshotNative();
    }

}
