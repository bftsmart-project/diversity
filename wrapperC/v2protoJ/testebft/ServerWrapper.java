/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package testebft;

import bftsmart.tom.MessageContext;
import bftsmart.tom.ServiceReplica;
import bftsmart.tom.server.defaultservices.DefaultSingleRecoverable;

/**
 *
 * @author alchieri
 */
public class ServerWrapper extends DefaultSingleRecoverable {
    Testcount.Counter estado;

    public ServerWrapper(int id){
         new ServiceReplica(id, this, this);
         estado = Testcount.Counter.newBuilder().setValue(0).build();
    }
    
    @Override
    public byte[] appExecuteOrdered(byte[] command, MessageContext msgCtx) {
        estado = Testcount.Counter.newBuilder().setValue(estado.getValue()+1).build();
        System.out.println("executeOrdered: " + estado.getValue());
        return estado.toByteArray();
    }

    @Override
    public byte[] executeUnordered(byte[] command, MessageContext msgCtx) {
        System.out.println("executeUnordered: " + estado.getValue());
        return estado.toByteArray();
    }

    @Override
    public void installSnapshot(byte[] state) {
    try {
        estado = Testcount.Counter.parseFrom(state);
    } catch (Exception e) { }

    }

    @Override
    public byte[] getSnapshot() {
        return estado.toByteArray();
    }


    
    
    
}
