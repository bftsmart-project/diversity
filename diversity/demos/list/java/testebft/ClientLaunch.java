/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package testebft;
import java.util.Scanner;
/**
 *
 * @author alchieri
 */
public class ClientLaunch {
    
     public static void main(String[] args){
          if(args.length < 1) {
            System.out.println("Use: java Serverlaunch <processId>");
            System.exit(-1);
        }
        ClientWrapper c = new ClientWrapper(Integer.parseInt(args[0]));

        int opt = 0;
Scanner sc = new Scanner(System.in);
Testcount.Counter oo = null;
byte[] resp;
        do {
System.out.println("1-ordered;2-unordered;3-sair (pressione CTRL-C depois)");
System.out.println("  ");
opt = sc.nextInt();
switch (opt) {
case 1:
resp = c.executeOrdered(new byte[0]);
try { oo = Testcount.Counter.parseFrom(resp); } catch (Exception e) {}
System.out.println("==> " + oo.getValue());
break;
case 2:
resp = c.executeUnordered(new byte[0]);
try { oo = Testcount.Counter.parseFrom(resp); } catch (Exception e) {}
System.out.println("==> " + oo.getValue());
break;
}

        } while (opt != 3);
    }

}
