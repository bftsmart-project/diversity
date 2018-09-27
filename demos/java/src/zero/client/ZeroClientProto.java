package bftsmart.diversity.demos.zero;
import bftbench.VazioOuterClass;
import bftbench.VazioOuterClass.Vazio;
import bftsmart.tom.ServiceProxy;
import java.io.*;
import com.google.protobuf.InvalidProtocolBufferException;


public class ZeroClientProto {
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

VazioOuterClass.Vazio.Builder reqst;
reqst = VazioOuterClass.Vazio
                    .newBuilder();

reqst.setTest(42);

byte[] msg = reqst.build().toByteArray();
System.out.println("--");
System.out.println(msg.length);


byte[] rsp = proxy.invokeOrdered(msg);
bftbench.VazioOuterClass.Vazio.parseFrom(rsp);
System.out.println(rsp.length);

byte[] rsp2 = proxy.invokeUnordered(msg);
bftbench.VazioOuterClass.Vazio.parseFrom(rsp2);
System.out.println(rsp2.length);

System.out.println("digite s para sair o enter para continuar");
lido = in.readLine();
} while (lido != "s");

proxy.close();

}

}
