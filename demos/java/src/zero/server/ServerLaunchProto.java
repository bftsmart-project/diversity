/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


package bftsmart.diversity.demos.zero;



import java.net.URL;
import java.net.URLClassLoader;

/**
 *
 * @author caio
 */
public class ServerLaunchProto {

    public static void main(String[] args){


          if(args.length < 2) {
            System.out.println("Use: java Serverlaunch <processId>");
            System.exit(-1);
        }
          for (int i = 0; i < args.length; i++) {
              System.out.println("-" + args[i] + "-");
          }
         new ZeroServerProto(Integer.parseInt(args[0]), Integer.parseInt(args[1]));

    }
}
