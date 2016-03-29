/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package bftsmart.demo.microbenchmarks.parallel.list;

/**
 *
 * @author caio
 */
public class ServerLaunch {
    
    
    public static void main(String[] args){
          if(args.length < 2) {
            System.out.println("Use: java Serverlaunch <processId>");
            System.exit(-1);
        }
          for (int i = 0; i < args.length; i++) {
              System.out.println("-" + args[i] + "-");
          }
          if (args.length > 2 && args[2].equals("pure")){
              System.out.println("Iniciando pre server");
              new TCCListServerPure(Integer.parseInt(args[0]), Integer.parseInt(args[1]));}
              else if (args.length > 2 && args[2].equals("normal")){
              System.out.println("Iniciando normal server");
              new TCCListServerNormal(Integer.parseInt(args[0]), Integer.parseInt(args[1]));}

              else if (args.length > 2 && args[2].equals("zero")){
              System.out.println("Iniciando zero server");
              new TCCZeroServer(Integer.parseInt(args[0]), Integer.parseInt(args[1]));}

              else if (args.length > 2 && args[2].equals("zeropure")){
              System.out.println("Iniciando zero server pure");
              new TCCZeroServerPure(Integer.parseInt(args[0]), Integer.parseInt(args[1]));}

else 
        new TCCListServer(Integer.parseInt(args[0]), Integer.parseInt(args[1]));
        
        
    }
}
