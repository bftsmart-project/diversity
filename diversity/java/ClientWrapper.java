/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package bftsmart.diversity;

import bftsmart.tom.ServiceProxy;

/**
 *
 * @author alchieri
 */
public class ClientWrapper {

    private ServiceProxy proxy;

    public ClientWrapper(int id) {
        proxy = new ServiceProxy(id);
    }

    public byte[] invokeOrdered(byte[] request) {
        return this.proxy.invokeOrdered(request);
    }

    public byte[] invokeUnordered(byte[] request) {
        return this.proxy.invokeUnordered(request);
    }

}
