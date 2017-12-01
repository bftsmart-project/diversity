package bftsmart.diversity.demos.zero;

import bftsmart.tom.ServiceProxy;
import java.io.*;



public class ZeroClient {
public static void main(String[] args) throws IOException {

	if (args.length < 1) {
		System.out.println("Usage: ... ListClient <process id>");
		System.exit(-1);
}

int id = Integer.parseInt(args[0]);

ServiceProxy proxy;
proxy = new ServiceProxy(id);
InputStreamReader converter = new InputStreamReader(System.in);
BufferedReader in = new BufferedReader(converter);
String lido;
do {
byte[] msg = new byte[]{};

byte[] rsp = proxy.invokeOrdered(msg);
byte[] rsp2 = proxy.invokeUnordered(msg);

System.out.println("digite s para sair o enter para continuar");
lido = in.readLine();
} while (lido != "s");

proxy.close();

}

}
